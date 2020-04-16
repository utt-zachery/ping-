#pragma once
#include <string>
#include "AbstractFlag.h"
#include "../utilities/FlagFacade.h"
#include "../utilities/PingCommand.h"
#include <thread>

//Concrete flag class that allows users to execute a custom command if PING requests are systematically failing
class FlagFacade;
class PingCommand;

class MinTtlFlag : public AbstractFlag {
public:
    MinTtlFlag();
    void parseCommand(int argc, const char ** arg);
    bool executeCommand(void* flagFacade);
    
private:
    bool isActive;
};
