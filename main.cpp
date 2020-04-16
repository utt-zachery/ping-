//
//  main.cpp
//  CloudflarePingTool
//
//  Created by Zachery Utt on 4/13/20.
//  Copyright © 2020 Zachery Utt. All rights reserved.
//

#include <iostream>
#include <thread>
#include <sys/socket.h>
#include "utilities/PingCommand.h"
#include "utilities/FlagFacade.h"

int main(int argc, const char * argv[]) {
    
    FlagFacade parser(argc, argv);
    
    //Perform any finite operations
    bool finiteOperations = parser.performFiniteOperations();
    
    //Initatie Auto-Ping
    if (!finiteOperations)
    {
        PingCommand pinger(&parser);
        while (true)
        {
            pinger.doPing();
            std::this_thread::sleep_for (std::chrono::milliseconds(parser.getWaitTime()));
            pinger.clear();
        }
    }
    
    return 0;
}
