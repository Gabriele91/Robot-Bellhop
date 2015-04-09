//
//  RobotSolver.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 27/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotBellhop_Solver_h
#define RobotBellhop_Solver_h

#include "Config.h"
#include "Robot.h"
#include "House.h"
#include "Context.h"
#include "Visitor.h"
#include "Timer.h"

#if 1
    #define MESSAGE(x)
#else
    #define MESSAGE(x) std::cout << x << std::endl;
#endif

#define TYPENAME_QUEUE_SOLVER( House )\
    using HSolver  = Solver< House > ;\
    using Queue    = typename Solver< House >::Queue ;\
    using ItQueue  = typename Solver< House >::ItQueue ;\
    using CtxHouse = typename Solver< House >::CtxHouse ;

namespace RobotBellhop
{

template < class House >
class Solver : public Visitor< Context < House > >
{
    
protected:
    
    typedef Context< House >                CtxHouse;
    typedef std::list< Context< House > >   Queue;
    typedef typename Queue::iterator        ItQueue;
    typedef std::list< Context< House > >   VSolution;
    
    House                    m_start;
    House                    m_final;
    VSolution                m_solution;
    bool                     m_doors_equals;
    size_t                   m_start_room;
    bool                     m_enable_cost;
    bool                     m_final_res;
    std::array< int, A_END > m_costs;
    
#ifdef ENABLE_ANALYSIS
    size_t                   m_count_nodes_visited;
    size_t                   m_max_queue_size;
    size_t                   m_count_nodes;
    double                   m_time_to_execute;
#endif
    
    virtual ItQueue extract_node(Queue& queue)=0;
    virtual ItQueue last_added(Queue& queue)=0;;
    virtual void pop(Queue& queue)=0;
    virtual CtxHouse& new_contex(ItQueue it,Queue& queue)=0;
    virtual bool succ(ItQueue it, Queue& queue) = 0;
    
    virtual CtxHouse& new_contex_aux(ItQueue it,Queue& queue)
    {
        //new context
        CtxHouse&  nctx=new_contex(it,queue);
        //inc level
        ++nctx.m_level;
#ifdef ENABLE_ANALYSIS
        //inc count of nodes
        m_count_nodes+=1;
        m_max_queue_size=std::max(m_max_queue_size, queue.size());
#endif
        //return new context
        return nctx;
    }
    
    bool delete_if_olready_exist(Queue& queue)
    {
#ifdef ENABLE_CYCLE_DETECTION
        ItQueue lastIt          =  last_added(queue);
        const CtxHouse& lastCtx = *lastIt;
        
        #pragma clang loop vectorize(enable)
        for(ItQueue it = queue.begin();
            it!=queue.end();
            ++it)
        {
            const CtxHouse& itCtx= *it;
            
            if( it != lastIt && itCtx == lastCtx )
            {
                pop(queue);
                return true;
            }
        }
#endif
        return false;
    }
    
    virtual ItQueue extract(Queue& queue)
    {
        //it's visited:
#ifdef ENABLE_ANALYSIS
        ++m_count_nodes_visited;
#endif
        return extract_node(queue);
    };
    
    void succ_aux(ItQueue it, Queue& queue)
    {
        //get current room
        const Room& current_room = it->current_room();
        //DEBUG
        MESSAGE(" -- -- -- -- -- ")
        //leaves
        if (it->a_obj_has_already_been_taken())
        {
            
            //new context
            auto& context=new_contex_aux(it,queue);
            //do action
            context.leaves_object();
            
            if NOT(delete_if_olready_exist(queue))
            {
                //added
                MESSAGE( "LEAVES IN " << context.m_robot.current_room() )
            }
        }
        //take
        else
        {
            //take all object
            for(size_t i=0; i!=current_room.size() ;++i)
            {
                //new context
                auto& context=new_contex_aux(it,queue);
                //do action
                context.take_object(i);
                
                if NOT(delete_if_olready_exist(queue))
                {
                    //added
                    MESSAGE( "TAKE OBJECT  " << i << " FROM " << context.m_robot.current_room() )
                }
            }
        }
        
        //move robot to room +1
        if (it->is_open_next())
        {
            //new context
            auto& context=new_contex_aux(it,queue);
            //do action
            context.go_next();
            
            if NOT(delete_if_olready_exist(queue))
            {
                //added
                MESSAGE( "ROOM +1, " << context.m_robot.current_room() )
            }
        }
        
        //move robot to room -1
        if (it->is_open_prev())
        {
            //new context
            auto& context=new_contex_aux(it,queue);
            //do action
            context.go_prev();
            
            if NOT(delete_if_olready_exist(queue))
            {
                //added
                MESSAGE( "ROOM -1, " << context.m_robot.current_room() )
            }
        }
        
        //try open/close a door
        if(it->object_taken_is_a_key())
        {
            if(it->can_open_prev())
            {
                //new context
                auto& context=new_contex_aux(it,queue);
                //do action
                context.try_open_prev();
                
                if NOT(delete_if_olready_exist(queue))
                {
                    //added
                    MESSAGE( "OPEN ROOM -1, " << context.m_robot.current_room() )
                }
            }
            
            if(it->can_open_next())
            {
                //new context
                auto& context=new_contex_aux(it,queue);
                //do action
                context.try_open_next();
                
                if NOT(delete_if_olready_exist(queue))
                {
                    //added
                    MESSAGE( "OPEN ROOM +1, " << context.m_robot.current_room() )
                }
            }
            //try close
            if(m_doors_equals)
            {
                if(it->can_close_prev())
                {
                    //new context
                    auto& context=new_contex_aux(it,queue);
                    //do action
                    context.try_close_prev();
                    
                    if NOT(delete_if_olready_exist(queue))
                    {
                        //added
                        MESSAGE( "CLOSE ROOM -1, " << context.m_robot.current_room() )
                    }
                }
                
                if(it->can_close_next())
                {
                    //new context
                    auto& context=new_contex_aux(it,queue);
                    //do action
                    context.try_close_next();
                    
                    if NOT(delete_if_olready_exist(queue))
                    {
                        //added
                        MESSAGE( "CLOSE ROOM +1, " << context.m_robot.current_room() )
                    }
                }
            }
        }
    }
    
    virtual void  start(Queue& q)
    {
        //add
        q.push_back(m_start);
        //bug (ref to tmp)
        q.begin()->m_parent =  nullptr;
        q.begin()->m_level  =  1;
        q.begin()->m_robot.init(m_start_room);
        //add cost table
        if(m_enable_cost)
        {
            q.begin()->m_costs = &m_costs[0];
        }
    }
    
    virtual bool  goal(ItQueue it)
    {
        if
            (
             ( m_doors_equals && (*it) == m_final )          ||  //equals rooms and door
             (!m_doors_equals && it->rooms_equals(m_final) )     //equals only rooms
            )
        {
            for (const CtxHouse* ptr=&(*it);
                 ptr != nullptr;
                 ptr = ptr->m_parent)
            {
                m_solution.push_front(*ptr);
            }
            return true;
        }
        return false;
    }
    
    
public:
    
    Solver(size_t start_room,
           const House& start,
           const House& final,
           bool  doors_equals = false)
    :m_start_room(start_room)
    ,m_start(start)
    ,m_final(final)
    ,m_doors_equals(doors_equals)
    ,m_enable_cost(false)
    ,m_final_res(false)
    {
#ifdef ENABLE_ANALYSIS
        m_max_queue_size =   0;
        m_count_nodes    =   0;
        m_time_to_execute= 0.0;
#endif
    }
    
    Solver(size_t                          start_room,
           const House&                    start,
           const House&                    final,
           const std::array< int, A_END >& costs,
           bool                            doors_equals = false)
    :m_start_room(start_room)
    ,m_start(start)
    ,m_final(final)
    ,m_doors_equals(doors_equals)
    ,m_enable_cost(true)
    ,m_final_res(false)
    ,m_costs(costs)
    {
#ifdef ENABLE_ANALYSIS
        m_max_queue_size =   0;
        m_count_nodes    =   0;
        m_time_to_execute= 0.0;
#endif
    }
    
    bool solver()
    {
#ifdef ENABLE_ANALYSIS
        //start
        m_count_nodes_visited = 0;
        m_count_nodes=1;
        m_max_queue_size=1;
        //count time of execute
        m_time_to_execute=
        Timer::time_of_execute([&]()
        {
            m_final_res=this->Visitor< CtxHouse >::solver();
        });
#else
        m_final_res=this->Visitor< CtxHouse >::solver();
#endif
        
        return m_final_res;
    }
    
    const VSolution& get_solution() const
    {
        return m_solution;
    }
    
    std::string to_string() const
    {
        
        std::stringstream sbuffer;
        
        if(m_final_res)
        {
            for(auto acontext : m_solution)
            {
                sbuffer << "\nLevel: "  << acontext.m_level << "\n{ \n";
                sbuffer << "\tAction: " << ActionsNames[acontext.m_action] << ";\n";
                sbuffer << "\tCost: "   << acontext.m_cost << ";\n";
                sbuffer << "\tRobot: { "<< acontext.m_robot.to_string() << " };\n";
                sbuffer << "\tHouse:\n" << acontext.m_house.to_string() << ";";
                sbuffer << "\n};\n";
            }
        }
        else
        {
                sbuffer << "Solution not found!\n";
        }
        
#ifdef ENABLE_ANALYSIS
        sbuffer << "Nodes generated: "  << m_count_nodes <<";\n";
        sbuffer << "Nodes visited: "  << m_count_nodes_visited <<";\n";
        sbuffer << "Time to execute: " << m_time_to_execute <<" ms;\n";
        sbuffer << "Max queue size: " << m_max_queue_size <<";\n";
#endif
        
        return sbuffer.str();
    }
    
};
    
};

#endif
