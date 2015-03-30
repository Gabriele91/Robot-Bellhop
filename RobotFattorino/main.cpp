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

const size_t kilobytes=1024;
const size_t megabytes=1048576;
const size_t gigabytes=1073741824;

int main(int argc, const char * argv[])
{
#ifdef _WIN32
    //THREAD_PRIORITY_HIGHEST
    if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL))
        std::cout << "Failed to set priority time critical ("<< GetLastError()<<")\n";
#endif
    //init alloc
    ObjectsMap::init_pool_alloc(megabytes*128);
    //parsing
    std::string errors;
    ConfigFile conf_file;
    if(!conf_file.init_from_file("config.txt",errors))
    {
        std::cout << errors;
        return -1;
    }
    conf_file.execute();
    std::cout << "complete execution!\n";
    return 0;
}


