#pragma once
#include "AbstractFlag.h"
#include <string>
#include "../exceptions/AbstractException.h"
#include "../exceptions/SyntaxException.h"
#include "../utilities/RollingSuccessTracker.h"
#include <cstdlib>

//Concrete flag class that allows users to execute a custom command if PING requests are systematically failing
class ErrorHandlerFlag : public AbstractFlag {

public:
    ErrorHandlerFlag();
    
    //Default behavior will be nothing. If the user passes in a command string and the threshold is reached, the command will be executed
    void handleError(AbstractException* error, RollingSuccessTracker& tracker);
    void parseCommand(int argc, const char ** arg);
private:
    //Command that will be executed if an error is encountered or if the failureThreshold is reached
    std::string commandString;
    
    //Command will only be executed if the PING failure percentage reaches this threshold
    double failureThreshold;
    
    bool isNumber(const std::string& s);
};
