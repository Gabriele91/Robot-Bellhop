//
//  Config.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 25/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_Config_h
#define RobotFattorino_Config_h


////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <list>
#include <cstring>
#include <memory>
////////////////////////////////////////////////////////////////

#define assert(x)
#define NOT(x) (!(x))
typedef signed long LONG;
#define ENABLE_ANALYSIS

////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////


#endif
