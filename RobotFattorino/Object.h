//
//  Object.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 25/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_Object_h
#define RobotFattorino_Object_h


enum TypeObject
{
    O_UNKNOW,
    O_KEY,
    O_ELEMENT0
};

//generic object
class Object
{
protected:
    
    unsigned int m_type { O_UNKNOW };
    
public:
    
    typedef std::shared_ptr<Object> ptr;
    typedef std::unique_ptr<Object> uptr;
    typedef std::weak_ptr<Object>   wptr;
    
    unsigned int type() const
    {
        return m_type;
    }
    ptr shader()
    {
        return ptr(this);
    }
    
    virtual Object::ptr copy() const = 0;
    virtual std::string name() const = 0;
    virtual bool operator==(const Object* obj) const = 0;
    virtual bool operator==(const Object& obj) const
    {
        return (*this) == (&obj);
    }
    
};

//class factory of components
class ObjectsMap
{
    //components
    typedef Object::ptr (*CreateObject)(const std::string& value);
    static std::map< std::string,CreateObject >* cmap;
    
public:
    
    static Object::ptr create(const std::string& name,const std::string& value);
    static void append(const std::string& name,CreateObject fun);

};

//class used for static registration of a object class
template<class T>
class ObjectItem
{
    
    static Object::ptr create(const std::string& value)
    {
        return (new T(value))->shader();
    }
    
    ObjectItem(const std::string& name)
    {
        ObjectsMap::append(name, ObjectItem<T>::create);
    }
    
public:
    
    
    static ObjectItem<T>& Instance(const std::string& name)
    {
        static ObjectItem<T> objectItem(name);
        return objectItem;
    }
    
};

#define REGISTERED_OBJECT(classname)\
namespace\
{\
    static const ObjectItem<classname>& _Robot_ ## classname ## _ObjectItem= ObjectItem<classname>::Instance( #classname );\
}




#endif
