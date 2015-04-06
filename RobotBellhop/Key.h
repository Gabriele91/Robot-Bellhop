//
//  Key.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 28/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotBellhop_Key_h
#define RobotBellhop_Key_h

#include "Config.h"
#include "Object.h"

namespace RobotBellhop
{

//key
class Key : public Object
{
    //attributes
    size_t m_id;
    //friend class
    friend class Door;
    
public:
    
    Key(size_t id=0)
    {
        //set id value
        m_id   = id;
        //set type
        m_type = O_KEY;
    }
    
    Key(const std::string& value)
    {
        //int to string
        std::istringstream ( value ) >> m_id ;
        //set type
        m_type = O_KEY;
    }
    
    size_t id_door()
    {
        return m_id;
    }
    
    virtual Object::ptr copy() const
    {
        return (new Key(*this))->shader();
    }
    
    virtual std::string name() const
    {
        return "Key: "+std::to_string(m_id);
    }
    
    virtual bool operator == (const Object* key) const
    {
        return key->type() == type() && ((Key*)key)->m_id == m_id;
    }
};
REGISTERED_OBJECT(Key);

};

#endif
