//
//  IDSolver.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 27/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_IDDFSSolver_h
#define RobotFattorino_IDDFSSolver_h

#include "Context.h"
#include "Solver.h"

namespace RobotBellhop
{

template < class House >
class IDDFSSolver : public Solver< House >
{
    
    TYPENAME_QUEUE_SOLVER( House )
    LONG m_level;
    LONG m_level_round_max;
    
protected:
    
    virtual ItQueue extract(Queue& queue)
    {
        return queue.begin();
    }
    
    virtual ItQueue last_added(Queue& queue)
    {
        return queue.begin();
    }
    
    virtual void pop(typename HSolver::Queue& queue)
    {
        queue.pop_front();
    }
    
    virtual CtxHouse& new_contex(ItQueue it, Queue& queue)
    {
        //push copy
        queue.push_front(*it);
        //get new node
        CtxHouse& ctx=*(queue.begin());
        //set parent
        return ctx;
    }
    
    virtual bool  succ(ItQueue it,Queue& queue)
    {
        //max level arrived
        m_level_round_max =  std::max(m_level_round_max, it->m_level);
        
        //expand only if not expanded and level < max level of round
        if ( !it->is_expanded() && it->m_level < m_level)
        {
            size_t size=queue.size();
            //expand
            it->expand();
            this->succ_aux(it,queue);
            //no childs? pop
            if(size==queue.size()) pop(queue);
        }
        //olready expanded? Pop
        else
        {
            pop(queue);
        }
        
        //queue is void and a DFS not stopped
        if ( !queue.size() && m_level_round_max == m_level)
        {
            //inc depth
            ++m_level;
            //restart
            HSolver::start(queue);
        }
        
        
        return true;
    }
    
    virtual void start(Queue& queue)
    {
        HSolver::start(queue);
        m_level = 1;
        m_level_round_max = 1;
    }
    
public:
    
    
    IDDFSSolver(size_t start_room,
                const House& start,
                const House& final,
                bool  doors_equals = false)
    :HSolver(start_room,
             start,
             final,
             doors_equals)
    {
        
    }
    
    IDDFSSolver(size_t                          start_room,
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
