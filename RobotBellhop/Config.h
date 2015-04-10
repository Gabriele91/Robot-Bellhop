//
//  Config.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 25/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotBellhop_Config_h
#define RobotBellhop_Config_h


////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <map>
#include <list>
#include <cstring>
#include <memory>
#ifdef _WIN32
    #include <Windows.h>
#endif
// Disable unknown pragma warnings
#pragma warning ( disable : 4068 )
// Disable deprecation warnings
#pragma warning(disable : 4996)
////////////////////////////////////////////////////////////////
namespace RobotBellhop
{

#define assert(x)
#define NOT(x) (!(x))
typedef signed long LONG;
#define ENABLE_ANALYSIS
#define ENABLE_POOL_OBJECT
#define ENABLE_STATIC_STRING_OBJECT
#define ENABLE_CYCLE_DETECTION
//class
class Object;
class Key;
class Door;
class Room;
class Robot;
class House;
//template class
template < const size_t size_house = 5 > class StaticHouse;
class DynamicHouse;
    
};
////////////////////////////////////////////////////////////////


#endif
