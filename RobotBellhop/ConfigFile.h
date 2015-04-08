//
//  ConfigFile.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 28/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotBellhop_ConfigFile_h
#define RobotBellhop_ConfigFile_h

#include "Config.h"
#include "Object.h"
#include "Room.h"
#include "House.h"
#include "Solver.h"
#include "Context.h"
#include <thread>

namespace RobotBellhop
{

enum TypeOfExecute
{
    EX_BFS,
    EX_DFS,
    EX_UC,
    EX_IDDFS
};
    
struct ConfigFile
{
    typedef Context< DynamicHouse > Context;
    typedef RobotBellhop::Solver< DynamicHouse > Solver;
    
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
    std::thread* parallel_execute(TypeOfExecute type, std::function< void (Solver*) > callback_end_comp);
    
};
    
};

#endif
