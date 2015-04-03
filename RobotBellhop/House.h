//
//  House.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 25/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_House_h
#define RobotFattorino_House_h

#include "Config.h"
#include "Object.h"
#include "Door.h"
#include "Room.h"

//casa
class House
{
protected:
    
    //vector of doors
    std::vector<Door> m_doors;
    
    //doors
    void create_doors()
    {
        //alloc
        m_doors.resize(size());
        //init
        for(size_t i=0; i!=size(); ++i)
        {
            m_doors[i].init(i);
        }
    }
    
    void create_doors(const std::vector<bool>& list_of_opened)
    {
        //assert
        assert(list_of_opened.size()<=size());
        //alloc
        m_doors.resize(size());
        //init
        for(size_t i=0; i!=size(); ++i)
        {
            m_doors[i].init(i, list_of_opened[i]);
        }
    }
    
    //init
    void init(size_t id, const  std::vector < Object::ptr >&  objects)
    {
        (*this)[id].init(id, objects);
    }
    
    void init_door(size_t id,bool status)
    {
        m_doors[id].init(id, status);
    }
    
public:
    
    virtual Room& at(size_t i)
    {
        return (*this)[i];
    }
    
    virtual const Room& at(size_t i) const
    {
        return (*this)[i];
    }
    
    virtual Door& at_door(size_t i)
    {
        return m_doors[i%size()];
    }
    
    virtual const Door& at_door(size_t i) const
    {
        return m_doors[i%size()];
    }
    
    virtual bool rooms_equals(const House& house) const
    {
        if(size()!=house.size()) return false;
        for(size_t i=0; i!=size(); ++i) if NOT((*this)[i]==house[i]){ return false; }
        return true;
    }
    virtual bool doors_equals(const House& house) const
    {
        if(size()!=house.size()) return false;
        for(size_t i=0; i!=size(); ++i) if NOT(m_doors[i]==house.m_doors[i]){ return false; }
        return true;
    }
    
    virtual bool operator == (const House* house) const
    {
        return (*this) == (*house);
    }
    
    virtual bool operator == (const House& house) const
    {
        if(size()!=house.size()) return false;
        //comparation of doors is fast
        for(size_t i=0; i!=size(); ++i)
            if NOT(m_doors[i] == house.m_doors[i]){ return false; }
        //comparation of rooms is slow
        for (size_t i = 0; i != size(); ++i)
            if NOT((*this)[i]==house[i])        { return false; }
        return true;
    }
    
    size_t get_door(size_t from,size_t to)
    {
        size_t        id_door = from;
        if(to < from) id_door = to;
        return id_door % size();
    }
    
    bool from_to(size_t from,size_t to)
    {
        return at_door(from).is_open();
    }
    
    bool from_to_try_open(size_t from,size_t to,Object::ptr obj)
    {
        if(obj->type()==O_KEY)
        {
            return at_door(get_door(from,to)).try_open((Key*)obj.get());
        }
        return false;
    }
   
    virtual Room& operator[] (size_t i) = 0;
    virtual const Room& operator[] (size_t i) const = 0;
    virtual size_t size() const = 0;
  
    std::string to_string() const
    {
        std::stringstream sbuffer;
        
        sbuffer << "\t{\n";
        for(size_t i=0; i!=size() ;++i)
        {
            sbuffer << "\n\t\t" << (at_door(i).is_open() ? "door: open;" : "door: close;" ) ;
            sbuffer << "\n\n\t\t" << i << ": " << at(i).to_string() << "; \n";
        }
        sbuffer << "\t}";
        
        return sbuffer.str();
    }
};


template < const size_t size_house >
class StaticHouse : public House
{
    
    std::array< Room, size_house > m_rooms;
    
    void init_rooms(std::initializer_list< std::vector < Object::ptr > > list)
    {
        //assert
        assert(list.size()<=size());
        //temp var
        size_t i=0;
        //init rooms
        for(auto objects:list)
        {
            init(i++,objects);
        }
        //create doors
        create_doors();
    }
    
    void init_rooms(std::initializer_list< std::vector < Object::ptr > > list,
                    const std::vector<bool>& list_of_opened)
    {
        //assert
        assert(list.size()<=size());
        //temp var
        size_t i=0;
        //init rooms
        for(auto objects:list)
        {
            init(i++,objects);
        }
        //create doors
        create_doors(list_of_opened);
    }
    
public:
    
    StaticHouse()
    {
        create_doors();
    }
    
    StaticHouse(std::initializer_list< std::vector < Object::ptr > > list)
    {
        init_rooms(list);
    }
    
    StaticHouse(std::initializer_list< std::vector < Object::ptr > > list,
                const std::vector<bool>& list_of_opened)
    {
        init_rooms(list,list_of_opened);
    }
    
    
    virtual Room& operator[] (size_t i)
    {
        return m_rooms[i%size()];
    }

    virtual const Room& operator[] (size_t i) const
    {
        return m_rooms[i%size()];
    }
    
    virtual size_t size() const
    {
        return size_house;
    }
    
};

class DynamicHouse : public House
{
    
    std::vector< Room > m_rooms;
    
public:
    
    DynamicHouse(){}
    
    
    void push( bool door, std::vector < Object::ptr > objects )
    {
        const size_t id=m_rooms.size();
        m_rooms.resize(id+1);
        m_doors.resize(id+1);
        init(id,objects);
        init_door(id, door);
    }
    
    
    virtual Room& operator[] (size_t i)
    {
        return m_rooms[i%size()];
    }
    
    virtual const Room& operator[] (size_t i) const
    {
        return m_rooms[i%size()];
    }
    
    virtual size_t size() const
    {
        return m_rooms.size();
    }
    
};

#endif
