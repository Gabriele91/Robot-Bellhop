//
//  StaticString.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 30/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotBellhop_StaticString_h
#define RobotBellhop_StaticString_h

#include "Config.h"
#include <cstring>

namespace RobotBellhop
{

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
        m_size=std::min(string.size(),capacity());
        std::strncpy(&m_buffer[0],string.c_str(),m_size);
        //safe end line
        m_buffer[m_size]='\0';
    }
    
    StaticString(const char* cstring)
    {
        size_t lencstr=std::strlen(cstring);
        m_size=std::min(lencstr,capacity());
        std::strncpy(&m_buffer[0],cstring,m_size);
        //safe end line
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

    operator const std::string () const
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
    
    StaticString<s_size> operator + (const StaticString<s_size>& str)
    {
        //copy this
        StaticString<s_size> output(*this);
        //new size
        output.m_size=std::min(size()+str.size(),capacity());
        //copy next part
        std::strncpy( &output.m_buffer[size()] , &str.m_buffer[0], output.m_size-size());
        //safe end line
        output.m_buffer[output.m_size]='\0';
        //return
        return output;
    }
};


template< const size_t size >
inline StaticString<size> operator + (const char* cstr,const StaticString<size>& str)
{
    StaticString<size> output= StaticString<size>(cstr)+str;
    return output;
}

template< const size_t size >
inline StaticString<size> operator + (const StaticString<size>& str,const char* cstr)
{
    StaticString<size> output= str+StaticString<size>(cstr);
    return output;
}

template< const size_t size >
inline StaticString<size> operator + (const std::string& sstr,const StaticString<size>& str)
{
    StaticString<size> output= StaticString<size>(sstr)+str;
    return output;
}

template< const size_t size >
inline StaticString<size> operator + (const StaticString<size>& str,const std::string& sstr)
{
    StaticString<size> output= str+StaticString<size>(sstr);
    return output;
}
    
};

#endif
