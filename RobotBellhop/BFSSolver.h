//
//  BFSSolver.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 27/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_BFSSolver_h
#define RobotFattorino_BFSSolver_h

#include "Context.h"
#include "Solver.h"

template < class House >
class BFSSolver : public Solver< House >
{
    
    TYPENAME_QUEUE_SOLVER( House )
    ItQueue  m_it;
    
protected:
    
    virtual ItQueue extract(Queue& queue)
    {
        return m_it;
    }
    
    virtual ItQueue last_added(Queue& queue)
    {
        return (--queue.end());
    }
    
    virtual void pop(typename HSolver::Queue& queue)
    {
        queue.pop_back();
    }
    
    virtual CtxHouse& new_contex(ItQueue it, Queue& queue)
    {
        //push copy
        queue.push_back(*it);
        //get new node
        CtxHouse& ctx=*(--queue.end());
        //set parent
        return ctx;
    }
    
    virtual bool  succ(ItQueue it,Queue& queue)
    {
        if NOT(it->is_expanded())
        {
            //expand
            it->expand();
            this->succ_aux(it,queue);
        }
        
        //go to next node
        ++m_it;
        
        //if is the last, exit from the loop
        return m_it!= queue.end();
    }
    
    virtual void start(Queue& queue)
    {
        HSolver::start(queue);
        m_it = queue.begin();
    }

    
public:
    
    
    BFSSolver(size_t start_room,
              const House& start,
              const House& final,
              bool  doors_equals = false)
    :HSolver(start_room,
             start,
             final,
             doors_equals)
    {
        
    }
    
    BFSSolver(size_t                          start_room,
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


#endif
