//
//  ConfigFile.cpp
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 28/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#include "ConfigFile.h"
#include "Parser.h"
#include "DFSSolver.h"
#include "BFSSolver.h"
#include "UCSolver.h"
#include "IDDFSSolver.h"
#include <fstream>


bool ConfigFile::init_from_file(const std::string& path,std::string& errors)
{
    //open
    std::ifstream file(path);
    if(!file.is_open()){ errors="I can't read the configuration file\n"; return false; }
    //read
    std::string file_str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    //crate parser
    Parser p;
    //parsing
    bool status=p.parsing_a_string(file_str,*this);
    //get errors
    if(!status) errors = p.errors_to_string();
    
    return status;
}
bool ConfigFile::init_from_string(const std::string& string,std::string& errors)
{
    //crate parser
    Parser p;
    //parsing
    bool status=p.parsing_a_string(string,*this);
    //get errors
    if(!status) errors = p.errors_to_string();
    
    return status;
}
void ConfigFile::execute()
{
    //dfs
    if(m_dfs.m_enable)
    {
        DFSSolver< DynamicHouse > dfsSolver(m_roobot_init,m_start,m_end, m_costs, m_doors_equals);
        dfsSolver.solver();
        
        std::ofstream out(m_dfs.m_file);
        out <<  dfsSolver.to_string();;
        out.close();
    }
    
    //bfs
    if(m_bfs.m_enable)
    {
        BFSSolver< DynamicHouse > bfsSolver(m_roobot_init,m_start,m_end, m_costs, m_doors_equals);
        bfsSolver.solver();
        
        std::ofstream out(m_bfs.m_file);
        out <<  bfsSolver.to_string();;
        out.close();
    }
    
    //uc
    if(m_uc.m_enable)
    {
        UCSolver< DynamicHouse > ucSolver(m_roobot_init,m_start,m_end, m_costs, m_doors_equals);
        ucSolver.solver();
        
        std::ofstream out(m_uc.m_file);
        out <<  ucSolver.to_string();;
        out.close();
    }
    
    //iddfs
    if(m_iddfs.m_enable)
    {
        IDDFSSolver< DynamicHouse > iddfsSolver(m_roobot_init,m_start,m_end, m_costs, m_doors_equals);
        iddfsSolver.solver();
        
        std::ofstream out(m_iddfs.m_file);
        out <<  iddfsSolver.to_string();;
        out.close();
    }

    
}