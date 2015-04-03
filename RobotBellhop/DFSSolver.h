//
//  DFSSolver.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 27/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_DFSSolver_h
#define RobotFattorino_DFSSolver_h

#include "Context.h"
#include "Solver.h"

template < class House >
class DFSSolver : public Solver< House >
{
    
    TYPENAME_QUEUE_SOLVER( House )
    
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
        if NOT(it->is_expanded())
        {
            size_t size=queue.size();
            //expand
            it->expand();
            this->succ_aux(it,queue);
            //no childs? pop
            if(size==queue.size()) pop(queue);
        }
        //olready expanded? Pop
        else pop(queue);
        //return true
        return true;
    }
    
public:
    
    
    DFSSolver(size_t start_room,
              const House& start,
              const House& final,
              bool  doors_equals = false)
    :HSolver(start_room,
             start,
             final,
             doors_equals)
    {
        
    }
    
    DFSSolver(size_t                          start_room,
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
