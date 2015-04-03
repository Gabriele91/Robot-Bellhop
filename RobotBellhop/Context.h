//
//  Context.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 27/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_Context_h
#define RobotFattorino_Context_h

#include "Config.h"
#include "Door.h"
#include "House.h"
#include "Robot.h"

namespace RobotBellhop
{

enum Actions
{
    A_START,
    
    A_MOVE_PREV,
    A_MOVE_NEXT,
    
    A_TAKE,
    A_LEAVES,
    
    A_OPEN_PREV,
    A_OPEN_NEXT,
    A_CLOSE_PREV,
    A_CLOSE_NEXT,
    
    A_END
};

static const char* ActionsNames[]=
{
    "START",
    "MOVE TO THE PREVIUS ROOM",
    "MOVE TO THE NEXT ROOM",
    "TAKE A OBJECT",
    "LEAVES A OBJECT",
    "OPEN DOOR OF THE PREVIOUS ROOM",
    "OPEN DOOR OF THE NEXT ROOM",
    "CLOSE DOOR OF THE PREVIOUS ROOM",
    "CLOSE DOOR OF THE NEXT ROOM",
    "END"
};

template < class House >
class Context
{
public:
    
    const Context* m_parent;
    House          m_house;
    Robot          m_robot;
    LONG           m_level;
    Actions        m_action;
    bool           m_expanded;
    int*           m_costs;
    int            m_cost;
    
    Context()
    {
        //void
        m_parent   = nullptr;
        m_level    = 0;
        m_action   = A_START;
        m_expanded = false;
        m_costs    = nullptr;
        m_cost     = 0;
    }
    
    Context(const House& house,
            Actions action=A_START,
            int*    costs =nullptr)
    {
        m_parent   = nullptr;
        m_house    = house;
        m_level    = 0;
        m_action   = action;
        m_expanded = false;
        m_costs    = costs;
        m_cost     = 0;
    }
    
    Context(const Context& context)
    {
        m_parent   = &context;
        m_house    = context.m_house;
        m_robot    = context.m_robot;
        m_level    = context.m_level;
        m_action   = context.m_action;
        m_costs    = context.m_costs;
        m_cost     = context.m_cost;
        //not expanded (default)
        m_expanded = false;
    }
    
    Room& current_room()
    {
        return m_house[m_robot.current_room()];
    }
    
    Room& current_room() const
    {
        return m_house[m_robot.current_room()];
    }
    
    bool a_obj_has_already_been_taken() const
    {
        return m_robot.has_an_object();
    }
    
    void take_object(size_t i)
    {
        Object::ptr obj=current_room().remove(i);
        m_robot.take_object(obj);
        add_action( A_TAKE );
    }
    
    void leaves_object()
    {
        Object::ptr obj=m_robot.leaves_object();
        current_room().push(obj);
        add_action( A_LEAVES );
    }
    
    void go_prev()
    {
        size_t n_rooms=m_house.size();
        m_robot.go_to(( m_robot.current_room() -1 + n_rooms ) % n_rooms );
        add_action( A_MOVE_PREV );
    }
    
    void go_next()
    {
        size_t n_rooms=m_house.size();
        m_robot.go_to((m_robot.current_room()+1) % n_rooms);
        add_action( A_MOVE_NEXT );

    }
    
    void expand()
    {
        assert( NOT(m_expanded) )
        m_expanded = true;
    }
    
    bool is_expanded() const
    {
        return m_expanded;
    }
    
    bool is_open_prev() const
    {
        return m_house.at_door( m_robot.current_room() ).is_open();
    }
    
    bool is_open_next() const
    {
        return m_house.at_door( m_robot.current_room() + 1 ).is_open();
    }
    
    bool object_taken_is_a_key() const
    {
        if  NOT(m_robot.object())
        {
            return false;
        }
        
        return (m_robot.object()->type() == O_KEY);
    }
    
    bool try_open(RoomDoorDir action)
    {
        if NOT(object_taken_is_a_key()) { return false; }
        //get id door
        const Key* key=((Key*) m_robot.object().get());
        //prev?
        if(action == PREV) return m_house.at_door( m_robot.current_room() ).try_open(key);
        //next?
        return m_house.at_door( m_robot.current_room() + 1 ).try_open(key);
    }
    
    bool try_close(RoomDoorDir action)
    {
        if NOT(object_taken_is_a_key()) { return false; }
        //get id door
        const Key* key=((Key*) m_robot.object().get());
        //prev?
        if(action == PREV) return m_house.at_door( m_robot.current_room() ).try_close(key);
        //next?
        return m_house.at_door( m_robot.current_room() + 1 ).try_close(key);
    }
    
    bool can_open(RoomDoorDir action) const
    {
        if NOT(object_taken_is_a_key()) { return false; }
        //get id door
        const Key* key=((Key*) m_robot.object().get());
        //prev?
        if(action == PREV) return m_house.at_door( m_robot.current_room() ).can_open(key);
        //next?
        return m_house.at_door( m_robot.current_room() + 1 ).can_open(key);
    }
    
    bool can_close(RoomDoorDir action) const
    {
        if NOT(object_taken_is_a_key()) { return false; }
        //get id door
        const Key* key=((Key*) m_robot.object().get());
        //prev?
        if(action == PREV) return m_house.at_door( m_robot.current_room() ).can_close(key);
        //next?
        return m_house.at_door( m_robot.current_room() + 1 ).can_close(key);
    }
    
    bool try_open_prev()
    {
        if (try_open(PREV))
        {
            add_action( A_OPEN_PREV );
            return true;
        }
        return false;
    }
    
    bool try_open_next()
    {
        if (try_open(NEXT))
        {
            add_action( A_OPEN_NEXT );
            return true;
        }
        return false;
    }
    
    bool can_open_prev() const
    {
        return can_open(PREV);
    }
    
    bool can_open_next() const
    {
        return can_open(NEXT);
    }
    
    bool try_close_prev()
    {
        if (try_close(PREV))
        {
            add_action( A_CLOSE_PREV );
            return true;
        }
        return false;
    }
    
    bool try_close_next()
    {
        if (try_close(NEXT))
        {
            add_action( A_CLOSE_NEXT );
            return true;
        }
        return false;
    }
    
    bool can_close_prev() const
    {
        return can_close(PREV);
    }
    
    bool can_close_next() const
    {
        return can_close(NEXT);
    }
    
    bool operator == (const House& house) const
    {
        return m_house == house;
    }
    
    bool rooms_equals (const House& house) const
    {
        return m_house.rooms_equals(house);
    }
    
    bool operator == (const Context& ctx) const
    {
        #if 1
        if NOT(m_robot.current_room() == ctx.m_robot.current_room())
        {
            return false;
        }
        //This 'if' is valid because the copy constructor do not copy the robot object,
        //that means the robot move object from context to another context.
        if NOT(m_robot.object() == ctx.m_robot.object())
        {
            return false;
        }
        #else
        if NOT(m_robot.current_room() == ctx.m_robot.current_room())
        {
            return false;
        }
        if NOT(m_robot.has_an_object() == ctx.m_robot.has_an_object())
        {
            return false;
        }
        if NOT(m_robot.has_an_object() && ((*m_robot.object()) == *ctx.m_robot.object()))
        {
            return false;
        }
        #endif
        if NOT(m_house == ctx.m_house)
        { 
            return false;
        }
        return true;
    }
    
    void add_action(Actions action)
    {
        m_action = action;
        if(m_costs) m_cost += m_costs[action];
    }
    
};
    
};

#endif
