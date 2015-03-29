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
#include "DFSSolver.h"
#include "BFSSolver.h"
#include "UCSolver.h"
#include "IDDFSSolver.h"

int main(int argc, const char * argv[])
{
    
    
    StaticHouse<5> houseInit({
        //0
        {
            ObjectsMap::create("Key","2"),
            ObjectsMap::create("Box","lettere"),
            ObjectsMap::create("Bread","0.5"),
        },
        //1
        {
            ObjectsMap::create("Box","pizza"),
            ObjectsMap::create("Bottle","poretti"),
        },
        //2
        {
        },
        //3
        {
        },
        //4
        {
            
        }
    },
    {
        //0
        true,
        //1
        true,
        //2
        false,
        //3
        true,
        //4
        true
    });
    
    StaticHouse<5> houseFinal({
        //0
        {
            ObjectsMap::create("Bread","0.5"),
        },
        //1
        {
            ObjectsMap::create("Box","lettere"),
        },
        //2
        {
            ObjectsMap::create("Key","2"),
        },
        //3
        {
            ObjectsMap::create("Box","pizza"),
            ObjectsMap::create("Bottle","poretti"),
        },
        //4
        {
        }
    },
    {
        //0
        true,
        //1
        true,
        //2
        true,
        //3
        true,
        //4
        true
    });

    
    const bool doors_equals = true;
    
    std::array< int , A_END > costs=
    {
        0,      //start
        
        1,       //move prev
        100,     //move next
        
        2,      //take
        2,      //leaves
        
        1,      //open prev
        1,      //open next
        4,      //close prev
        4       //close next
    };
    
    DFSSolver< StaticHouse<5> > dfsSolver(1,houseInit,houseFinal, costs,doors_equals);
    dfsSolver.solver();
    
    std::cout << "\n--------------------------------\n";
    std::cout << "DFS SOL:\n\n";
    std::cout << dfsSolver.to_string();
    
    BFSSolver< StaticHouse<5> > bfsSolver(1,houseInit,houseFinal, costs,doors_equals);
    bfsSolver.solver();

    std::cout << "\n--------------------------------\n";
    std::cout << "BFS SOL:\n\n";
    std::cout << bfsSolver.to_string();
    
    UCSolver< StaticHouse<5> > ucSolver(1,houseInit,houseFinal, costs,doors_equals);
    ucSolver.solver();
    
    std::cout << "\n--------------------------------\n";
    std::cout << "UC SOL:\n\n";
    std::cout << ucSolver.to_string();
    
    
    IDDFSSolver< StaticHouse<5> > iddfsSolver(1,houseInit,houseFinal, costs,doors_equals);
    iddfsSolver.solver();
    
    std::cout << "\n--------------------------------\n";
    std::cout << "IDDFS SOL:\n\n";
    std::cout << iddfsSolver.to_string();
    
    return 0;
}
