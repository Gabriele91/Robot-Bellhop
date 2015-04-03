//
//  ObjectPoolAlloc.cpp
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 30/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#include "ObjectPoolAlloc.h"

namespace RobotBellhop
{

//singleton
ObjectPoolAlloc* ObjectPoolAlloc::m_global_pool_alloc = nullptr;

//set allocation
void ObjectPoolAlloc::init(size_t block_size,size_t max_size)
{
    assert(NOT(m_global_pool_alloc));
    //compute max_n_elements
    size_t n_elements= max_size / (block_size+sizeof(ObjectPoolAlloc::BlockAlloc));
    assert(n_elements);
    //alloc
    m_global_pool_alloc = new ObjectPoolAlloc(block_size,n_elements);
    //dealloc
    atexit([]()
    {
        delete m_global_pool_alloc;
    });
}
    
    
};
