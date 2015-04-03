//
//  Timer.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 28/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_Timer_h
#define RobotFattorino_Timer_h

#if defined(_MSC_VER)
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

#include <functional>
namespace RobotBellhop
{

namespace Timer
{
#if defined(_MSC_VER)
    typedef __int64    bit64;
    typedef __int64    byte16;
#else
    typedef long long  bit64;
    typedef long long  byte16;
#endif
    
    inline bit64 get_time_ticks()
    {
        bit64 val;
#if !defined(_MSC_VER)
        timeval timeVal;
        
        // http://linux.die.net/man/2/gettimeofday
        gettimeofday( &timeVal, NULL );
        
        val = (bit64)timeVal.tv_sec * (1000*1000) + (bit64)timeVal.tv_usec;
#else
        QueryPerformanceCounter( (LARGE_INTEGER *)&val );
#endif
        return val;
    }

    inline double get_time()
    {
        static double coe;
#if !defined(_MSC_VER)
        coe = 1.0 / 1000.0;
#else
        static bit64 freq;
        
        if ( freq == 0 )
        {
            QueryPerformanceFrequency( (LARGE_INTEGER *)&freq );
            coe = 1000.0 / freq;
        }
#endif
        
        return get_time_ticks() * coe;
    }

    inline double time_of_execute(std::function<void(void)> task)
    {
		double start = get_time();
        task();
        return get_time()-start;
    }
}
    
};

#endif
