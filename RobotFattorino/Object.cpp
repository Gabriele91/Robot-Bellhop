//
//  Object.cpp
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 28/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//
#include "Config.h"
#include "Object.h"
//static map
std::map< std::string, ObjectsMap::CreateObject >* ObjectsMap::cmap=nullptr;
//registration of a object
void ObjectsMap::append(const std::string& name,ObjectsMap::CreateObject fun)
{
    //map exist?
    if(!cmap) cmap=new std::map< std::string , ObjectsMap::CreateObject>();
    //assert...
    assert(cmap->find(name)==cmap->end());
    assert(fmap->find(name)==fmap->end());
    //append component
    (*cmap)[name]=fun;
}
//create a object
Object::ptr ObjectsMap::create(const std::string& name, const std::string& value)
{
    //assert...
    assert(cmap);
    //find
    auto it=cmap->find(name);
    //exist?
    if(it!=cmap->end())
    {
        return it->second(value);
    }
    //else null
    return nullptr;
}
