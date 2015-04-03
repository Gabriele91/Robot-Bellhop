//
//  StaticString.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 30/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_StaticString_h
#define RobotFattorino_StaticString_h

#include "Config.h"
#include <cstring>

template< const size_t s_size >
class StaticString
{

    size_t m_size;
    std::array<char,s_size+1> m_buffer;

public:
    
    StaticString()
    {
        m_buffer[0]='\0';
        m_buffer[s_size]='\0';
        m_size = 0;
    }
    
    StaticString(const std::string& string)
    {
        m_size=std::min(string.size()+1,capacity());
        std::strncpy(&m_buffer[0],string.c_str(),m_size);
        m_buffer[m_size]='\0';
    }
    
    size_t size() const
    {
        return m_size;
    }
    
    size_t capacity()
    {
        return s_size;
    }
    
    operator std::string () const
    {
        return std::string(&m_buffer[0]);
    }
    
    operator const char* () const
    {
        return &m_buffer[0];
    }
    
    bool operator == (const StaticString& compare) const
    {
        return m_size==compare.m_size && std::strcmp(&m_buffer[0],&compare.m_buffer[0])==0;
    }
    
};

#endif
