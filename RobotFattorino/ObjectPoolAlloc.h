//
//  ObjectPoolAlloc.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 30/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_ObjectPoolAlloc_h
#define RobotFattorino_ObjectPoolAlloc_h

#include "Config.h"
#include <memory>
//
class ObjectsMap;
//
class ObjectPoolAlloc
{
    //attributes
    size_t m_next       { 0       };
    size_t m_all_block  { 0       };
    size_t m_block      { 0       };
    size_t m_size       { 0       };
    size_t m_count      { 0       };
    size_t m_array_size { 0       };
    char*  m_bytes      { nullptr };
    bool*  m_used       { nullptr };
    //header of a allocation
    struct BlockAlloc
    {
        size_t m_alloc_id;
    };
    //search a new allocation
    bool search_alloc(size_t from,size_t& out_alloc)
    {
        size_t new_alloc = 0;
        //search
        for(size_t i=0; i!=m_size ; ++i)
        {
            //set id of new alloc
            new_alloc = (i+from)%m_size;
            //try
            if NOT(m_used[new_alloc])
            {
                out_alloc=new_alloc;
                return true;
            }
        }
        return false;
    }
    //init
    ObjectPoolAlloc(size_t block,size_t size)
    :m_block(block)
    ,m_size(size)
    {
        m_all_block =(m_block+sizeof(BlockAlloc));
        m_array_size= m_all_block*m_size;
        m_bytes     = (char*)std::malloc(m_array_size);
        m_used      = (bool*)std::malloc(m_size*sizeof(bool));
        std::memset(m_bytes, 0, m_array_size);
        std::memset(m_used,  0, m_size*sizeof(bool));
    }
    //dealloc
    ~ObjectPoolAlloc()
    {
        std::free(m_bytes);
        std::free(m_used);
    }
    
    //firend class
    friend class ObjectsMap;
    //singleton
    static ObjectPoolAlloc* m_global_pool_alloc;
    //set allocation
    static void init(size_t block_size,size_t max_size);
    
    void* _alloc()
    {
        //new alloc id
        size_t new_alloc=0;
        //get new alloc id
        if NOT(search_alloc(m_next,new_alloc)){ throw(std::bad_alloc()); }
        //save last id allocated
        m_used[new_alloc] = true;
        m_next            = new_alloc+1;
        //inc allocation count
        ++m_count;
        //save id
        char*  byte_block=&(m_bytes[ m_all_block*new_alloc ]);
        ((BlockAlloc*)(byte_block))->m_alloc_id = new_alloc;
        //return pointer
        return (byte_block+sizeof(BlockAlloc));
    }
    void _dealloc( void* ptr )
    {
        //get id
        char*  byte_block=((char*)ptr) - sizeof(BlockAlloc);
        BlockAlloc& block= *((BlockAlloc*)(byte_block));
        //is olready dealloc
        if NOT(m_used[block.m_alloc_id]){ throw(std::bad_alloc()); }
        //dec allocation count
        --m_count;
        //dealloc
        m_used[block.m_alloc_id]=false;
    }
    
    
public:

    static void* alloc()
    {
        assert(m_global_pool_alloc);
        return m_global_pool_alloc->_alloc();
    }
    
    static void dealloc( void* ptr )
    {
        assert(m_global_pool_alloc);
        m_global_pool_alloc->_dealloc(ptr);
    }

    
};

#endif
