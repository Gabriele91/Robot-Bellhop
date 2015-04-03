//
//  Door.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 25/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_Door_h
#define RobotFattorino_Door_h

#include "Config.h"
#include "Object.h"
#include "Key.h"

namespace RobotBellhop
{

class Door
{
    //attributes
    bool    m_open{ true };
    size_t  m_id  { 0    };
    
    //init
    void init(size_t id, bool open=true)
    {
        m_id    = id;
        m_open  = open;
    }
    
    //friend class
    friend class House;
    
public:
    
    Door(){}
    Door(size_t id, bool open=true)
    {
        init(id,open);
    }
    
    bool is_open() const
    {
        return m_open;
    }
    
    bool can_open(const Key* key) const
    {
        return key->m_id == m_id && !m_open;
    }
    
    bool can_open(const Key& key) const
    {
        return key.m_id == m_id && !m_open;
    }
    
    bool try_open(const Key* key)
    {
        if(key->m_id == m_id) m_open=true;
        return is_open();
    }
    
    bool try_open(const Key& key)
    {
        if(key.m_id == m_id) m_open=true;
        return is_open();
    }
    
    bool can_close(const Key* key) const
    {
        return key->m_id == m_id && m_open;
    }
    
    bool can_close(const Key& key) const
    {
        return key.m_id == m_id && m_open;
    }
    
    bool try_close(const Key* key)
    {
        if(key->m_id == m_id) m_open=false;
        return !is_open();
    }
    
    bool try_close(const Key& key)
    {
        if(key.m_id == m_id) m_open=false;
        return !is_open();
    }
    
    bool operator == (const Door& door) const
    {
        return m_id == door.m_id && m_open==door.m_open;
    }
};
    
};

#endif
