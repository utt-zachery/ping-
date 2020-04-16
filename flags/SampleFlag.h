#pragma once
#include <string>
#include "AbstractFlag.h"
#include "../utilities/FlagFacade.h"
#include "../utilities/PingCommand.h"
#include <thread>
#include <cmath>

//Concrete flag class that allows users to execute a custom command if PING requests are systematically failing
class FlagFacade;
class PingCommand;

class SampleFlag : public AbstractFlag {
    
public:
    SampleFlag();
    void parseCommand(int argc, const char ** arg);
    bool executeCommand(void* flagFacade);
    
private:
    int sampleSize;
    bool isActive;
    bool isNumber(const std::string& s);
};
