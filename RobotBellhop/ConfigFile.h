//
//  ConfigFile.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 28/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_ConfigFile_h
#define RobotFattorino_ConfigFile_h

#include "Config.h"
#include "Object.h"
#include "Room.h"
#include "House.h"
#include "Context.h"

namespace RobotBellhop
{

struct ConfigFile
{
    size_t m_roobot_init { 0     };
    bool   m_doors_equals{ false };
    
    struct TreeOuput
    {
        bool m_enable { false };
        std::string m_file;
    };
    
    TreeOuput m_dfs;
    TreeOuput m_bfs;
    TreeOuput m_uc;
    TreeOuput m_iddfs;
    
    DynamicHouse m_start;
    DynamicHouse m_end;
    
    std::array< int , A_END > m_costs;
    
    bool init_from_file(const std::string& path,std::string& errors);
    bool init_from_string(const std::string& string,std::string& errors);
    void execute();
    
};
    
};

#endif
