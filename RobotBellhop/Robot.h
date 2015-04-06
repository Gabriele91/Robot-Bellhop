//
//  Robot.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 26/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotBellhop_Robot_h
#define RobotBellhop_Robot_h

#include "Config.h"
#include "Object.h"

namespace RobotBellhop
{

class Robot
{
    //object take
    Object::ptr m_obj    {nullptr};
    //current room
    size_t      m_room_id{   0   };
    
public:
    
    void init(size_t room)
    {
        m_room_id = room;
        m_obj     = nullptr;
    }
    
    void take_object(Object::ptr obj)
    {
        m_obj = obj;
    }
    
    Object::ptr leaves_object()
    {
        Object::ptr tmp_obj=m_obj;
        m_obj=nullptr;
        return tmp_obj;
    }
    
    Object::ptr object() const
    {
        return m_obj;
    }
    
    bool has_an_object() const
    {
        return m_obj!=nullptr;
    }
    
    size_t current_room() const
    {
        return m_room_id;
    }
    
    void go_to(size_t room)
    {
        m_room_id = room;
    }
    
    std::string to_string() const
    {
        std::stringstream buffer;
        
        buffer << " Room: "  << m_room_id<<";";
        if( m_obj ) buffer <<" Object: { " << m_obj->name() << " };";
        
        return buffer.str();
    }
    
};
    
};

#endif
