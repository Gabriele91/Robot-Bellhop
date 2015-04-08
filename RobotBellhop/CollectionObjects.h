//
//  CollectionObjects.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 28/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotBellhop_CollectionObjects_h
#define RobotBellhop_CollectionObjects_h

#include "Config.h"
#include "Object.h"
#include "StaticString.h"

namespace RobotBellhop
{

namespace Default
{
    #ifdef ENABLE_STATIC_STRING_OBJECT
        typedef StaticString< 50 > string;
    #else
        typedef std::string string;
    #endif
}
enum TypeObjectStandard
{
    O_BOX=O_ELEMENT0,
    O_BOTTLE,
    O_BREAD
};

//Box object
class Box : public Object
{
    //attributes
    Default::string m_name;
    //friend class
    friend class Door;
    
public:
    
    Box(const std::string& name)
    {
        m_name = name;
        m_type = O_BOX;
    }
    
    virtual Object::ptr copy() const
    {
        return (new Box(*this))->shader();
    }
    
    virtual std::string name() const
    {
        return "Box: "+m_name;
    }
    
    virtual std::string class_name() const
    {
        return "Box";
    }
    
    virtual bool operator==(const Object* obj) const
    {
        return obj->type() == type() && ((Box*)obj)->m_name == m_name;
    }
    
};
REGISTERED_OBJECT(Box);

//Bottle object
class Bottle : public Object
{
    //attributes
    Default::string m_name;
    //friend class
    friend class Door;
    
public:
    
    Bottle(const std::string& name)
    {
        m_name = name;
        m_type = O_BOTTLE;
    }
    
    virtual Object::ptr copy() const
    {
        return (new Bottle(*this))->shader();
    }
    
    virtual std::string name() const
    {
        return "Bottle: "+m_name;
    }
    
    virtual std::string class_name() const
    {
        return "Bottle";
    }
    
    virtual bool operator==(const Object* obj) const
    {
        return obj->type() == type() && ((Bottle*)obj)->m_name == m_name;
    }
    
};
REGISTERED_OBJECT(Bottle);

//Bread object
class Bread : public Object
{
    //attributes
    double m_kg;
    //friend class
    friend class Door;
    
public:
    
    Bread(double kg=0.0f)
    {
        //set id value
        m_kg   = kg;
        //set type
        m_type = O_BREAD;
    }
    
    Bread(const std::string& value)
    {
        //int to string
        std::istringstream ( value ) >> m_kg ;
        //set type
        m_type = O_BREAD;
    }
    
    virtual Object::ptr copy() const
    {
        return (new Bread(*this))->shader();
    }
    
    virtual std::string name() const
    {
        return "Bread: "+std::to_string(m_kg)+" kg";
    }
    
    virtual std::string class_name() const
    {
        return "Bread";
    }
    
    virtual bool operator==(const Object* obj) const
    {
        return obj->type() == type() && ((Bread*)obj)->m_kg == m_kg;
    }
    
};
REGISTERED_OBJECT(Bread);

};

#endif
