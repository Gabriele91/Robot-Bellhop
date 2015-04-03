//
//  main.cpp
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 24/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#include "Config.h"
#include "Object.h"
#include "CollectionObjects.h"
#include "ConfigFile.h"


int main(int argc, const char * argv[])
{
    
#ifdef _WIN32
    //THREAD_PRIORITY_HIGHEST
    if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL))
        std::cout << "Failed to set priority time critical ("<< GetLastError()<<")\n";
#endif
    //default file
    std::string file_input("config.txt");
    //else
    if(argc > 1)
    {
        //arg 1 to std::string
        std::string arg_0(argv[0]);
        std::string arg_1(argv[1]);
        
        //get version
        if(arg_1=="-v" || arg_1=="--version")
        {
            std::cout << "version 1.0\n";
            return 0;
        }
        
        //help
        else if(arg_1=="-h" || arg_1=="--help")
        {
            std::cout << "RobotBellhop [commands]\n\n";
            std::cout << "COMMANDS:\n";
            std::cout << "  -h, --help     help,\n";
            std::cout << "  -v, --version  version of the program,\n";
            std::cout << "  <input>        input file [default is config.txt]\n";
            return 0;
        }
        
        //it is a input file
        else
        {
            file_input=arg_1;
        }
    }
    //init pool allocator
    const size_t megabytes=1048576;
    RobotBellhop::ObjectsMap::init_pool_alloc(megabytes*128);
    //parsing
    std::string errors;
    RobotBellhop::ConfigFile conf_file;
    if(!conf_file.init_from_file(file_input,errors))
    {
        std::cout << errors;
        return -1;
    }
    //execute
    conf_file.execute();
    //end execute
    std::cout << "complete execution!\n";
    return 0;
}


