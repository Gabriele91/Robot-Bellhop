//
//  Room.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 25/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotBellhop_Room_h
#define RobotBellhop_Room_h

#include "Config.h"
#include "Object.h"
#include "Door.h"

namespace RobotBellhop
{

enum RoomDoorDir
{
    PREV = 0,
    NEXT = 1
};

//stanza
class Room
{
    //attributes
    size_t m_id{ 0 };
    std::vector < Object::ptr  > m_objects;
    typedef  std::vector < Object::ptr  >::iterator ItObjects;
    typedef  std::vector < Object::ptr  >::const_iterator CItObjects;
    //init
    void init(size_t id, const  std::vector < Object::ptr  >&  objects)
    {
        m_id     = id;
        m_objects= objects;
    }
    //friend class
    friend class House;
    
public:
    
    Room(){}
    Room(const Room& room)
    {
        //copy all objects
        for(auto obj : room.m_objects)
        {
            m_objects.push_back(obj->copy());
        }
    }
    
    Object::ptr remove(size_t i)
    {
        ItObjects it   =(m_objects.begin()+i);
        Object::ptr obj=*it;
        m_objects.erase(it);
        return obj;
    }
    
    Object::ptr pop()
    {
        return *m_objects.erase(m_objects.begin()+size());
    }
    
    void push(const Object::ptr obj)
    {
        m_objects.push_back(obj);
    }
    
    Object& operator[](const size_t object) const
    {
        return  *m_objects[object];
    }
    
    ItObjects begin()
    {
        return m_objects.begin();
    }
    
    CItObjects begin() const
    {
        return m_objects.begin();
    }
    
    ItObjects end()
    {
        return m_objects.end();
    }
    
    CItObjects end() const
    {
        return m_objects.end();
    }
    
    size_t size() const
    {
        return m_objects.size();
    }
    
    long find(Object::ptr obj) const
    {
        for(size_t i=0; i!=m_objects.size();++i)
        {
            if((*m_objects[i])==*obj)  return (long)i;
        }
        return -1;
    }
    
    Object::ptr find_and_remove(Object::ptr obj)
    {
        for(size_t i=0; i!=m_objects.size();++i)
        {
            if((*m_objects[i])==*obj) return remove( i );
        }
        return nullptr;
    }
    
    size_t get_id() const
    {
        return m_id;
    }
    
    bool operator == (const Room& room) const
    {
        if(m_id!=room.m_id) return false;
        ////////NOT EQUAL NUMBER OF OBJECTS?
        if (m_objects.size() != room.m_objects.size()) 
            return false;
        ////////ALL MY OBJS IN ROOM?
        for(Object::ptr this_it  : m_objects)
            if( room.find(this_it) < 0 ) 
                return false;
        ////////ALL ROOM OBJS IN ME? (unnecessary)
        #if 0
        for(Object::ptr this_it  : room.m_objects)
            if( find(this_it) < 0 ) 
                return false;
        #endif

        return true;
    }
    
    std::string to_string() const
    {
        std::stringstream sbuffer;
        
        sbuffer << "{ ";
        for(auto obj : m_objects) sbuffer << obj->name() << "; ";
        sbuffer << "}";
        
        return sbuffer.str();
    }
};

};

#endif
