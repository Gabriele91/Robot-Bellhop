//
//  Visitor.h
//  Brocche_2.0
//
//  Created by Gabriele Di Bari on 19/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#pragma once

#include <queue>
#include <list>

namespace RobotBellhop
{

template< typename State,
          typename Queue = std::list< State > >
class Visitor
{
    
protected:
    
    virtual typename Queue::iterator extract(Queue& queue) = 0;
    virtual bool  goal(typename Queue::iterator it) = 0;
    virtual bool  succ(typename Queue::iterator it, Queue& queue) = 0;
    virtual void  start(Queue& queue) = 0;
    bool solver()
    {
        Queue queue;
        start(queue);
        
        while (queue.size())
        {
            typename Queue::iterator current = extract(queue);
            if  ( goal(current) ) return true;
            if(!succ(current, queue)) return false;
        }
        return false;
    }
    
};

};