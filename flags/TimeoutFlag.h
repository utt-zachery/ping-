#pragma once
#include "AbstractFlag.h"
#include "SyntaxException.h"
#include <string>

class TimeoutFlag : public AbstractFlag
{
public:
    TimeoutFlag();
    
    //Default behavior will be set to 255. If the user passes in a command string and the threshold is reached, the command will be executed
    void parseCommand(int argc, const char** arg);
    
    int getTimeout();
    
private:
    int timeout;
    bool isNumber(const std::string& s);
};
