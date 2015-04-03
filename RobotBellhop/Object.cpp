//
//  Object.cpp
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 28/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//
#include "Config.h"
#include "Object.h"
#include "ObjectPoolAlloc.h"

namespace RobotBellhop
{

//object
#ifdef ENABLE_POOL_OBJECT
void* Object::operator new     ( std::size_t count )
{
    return ObjectPoolAlloc::alloc();
}
void  Object::operator delete  ( void* ptr )
{
    ObjectPoolAlloc::dealloc(ptr);
}
#endif

//static map
size_t ObjectsMap::m_max_size = 0;
std::map< std::string, ObjectsMap::CreateObject >* ObjectsMap::m_cmap=nullptr;

//init pool
void ObjectsMap::init_pool_alloc(size_t max_pool_size)
{
#ifdef ENABLE_POOL_OBJECT
    ObjectPoolAlloc::init(m_max_size, max_pool_size);
#endif
}
//return max size of a object
size_t ObjectsMap::get_max_size_of_a_object()
{
    return m_max_size;
}
//registration of a object
void ObjectsMap::append(const std::string& name,ObjectsMap::CreateObject fun, size_t size)
{
    //calc max size
    m_max_size = std::max(m_max_size,size);
    //map exist?
    if(!m_cmap) m_cmap=new std::map< std::string , ObjectsMap::CreateObject>();
    //assert...
    assert(m_cmap->find(name)==m_cmap->end());
    //append component
    (*m_cmap)[name]=fun;
}
//create a object
Object::ptr ObjectsMap::create(const std::string& name, const std::string& value)
{
    //assert...
    assert(m_cmap);
    //find
    auto it=m_cmap->find(name);
    //exist?
    if(it!=m_cmap->end())
    {
        return it->second(value);
    }
    //else null
    return nullptr;
}

};