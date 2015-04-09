//
//  UCSolver.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 27/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotBellhop_UCSolver_h
#define RobotBellhop_UCSolver_h


#include "Context.h"
#include "Solver.h"

namespace RobotBellhop
{

template < class House >
class UCSolver : public Solver< House >
{
    
    TYPENAME_QUEUE_SOLVER( House )
    ItQueue  m_it;
    ItQueue  m_last_added;
    
protected:
    
    virtual ItQueue extract_node(Queue& queue)
    {
        return m_it;
    }
    
    virtual ItQueue last_added(Queue& queue)
    {
        return m_last_added;
    }
    
    virtual void pop(typename HSolver::Queue& queue)
    {
        queue.erase(m_last_added);
    }
    
    virtual CtxHouse& new_contex(ItQueue it, Queue& queue)
    {
        //push copy
        queue.push_back(*it);
        //get last node
        m_last_added=(--queue.end());
        //ref of last node
        CtxHouse& ctx= *m_last_added;
        //return
        return ctx;
    }
    
    static bool compareIt(const CtxHouse& lit,
                          const CtxHouse& rit)
    {
        return lit.m_cost < rit.m_cost;
    }
    
    void sort(ItQueue it,Queue& queue)
    {
        //take a part of list and sort
        Queue temp;
        temp.splice(temp.end(), queue, it, queue.end());
        temp.sort(compareIt);
        //add sorted part to list
        queue.splice(queue.end(), temp, temp.begin(), temp.end());

    }
        
    virtual bool  succ(ItQueue it,Queue& queue)
    {
        if NOT(it->is_expanded())
        {
            //expand
            it->expand();
            this->succ_aux(it,queue);
        }
        //next node (n.b it == m_it)
        ++it;
        //is the last? Exit from loop
        if( it == queue.end() ) return false;
        //sort queue
        sort(it,queue);
        //next (sorted)
        ++m_it;
        //return
        return  true;
    }
    
    virtual void start(Queue& queue)
    {
        HSolver::start(queue);
        m_it          = queue.begin();
        m_last_added  = queue.begin();
    }
    
    
public:
    
    
    UCSolver(size_t start_room,
             const House& start,
             const House& final,
             bool  doors_equals = false)
    :HSolver(start_room,
             start,
             final,
             doors_equals)
    {
        
    }
    
    UCSolver(size_t                          start_room,
             const House&                    start,
             const House&                    final,
             const std::array< int, A_END >& costs,
             bool                            doors_equals = false)
    :HSolver(start_room,
             start,
             final,
             costs,
             doors_equals)
    {
        
    }
    
};

};

#endif
