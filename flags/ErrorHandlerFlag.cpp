#include "ErrorHandlerFlag.h"

ErrorHandlerFlag::ErrorHandlerFlag()
{
    this->commandString = "";
    this->failureThreshold = 0.0;
}

void ErrorHandlerFlag::parseCommand(int argc, const char** argv)
{
    for (int i=1; i < argc; i++)
    {
        if (std::string(argv[i]) == "-f")
        {
            this->commandString =argv[i+1];
        }
        else if (std::string(argv[i]) == "-e")
        {
            if (!isNumber(std::string(argv[i+1])))
            {
                throw new SyntaxException("\"" + std::string(argv[i+1]) + "\" (double expected)");
            }
            this->failureThreshold = std::stod(argv[i+1]);
        }
    }
}

bool ErrorHandlerFlag::isNumber(const std::string& s)
{
    char* end = nullptr;
    double val = strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && val != std::numeric_limits<double>::max() && val >= 0;
}

void ErrorHandlerFlag::handleError(AbstractException* error, RollingSuccessTracker& tracker)
{
    if (this->commandString.length() > 0)
    {
        //Fatal errors must be reported. Only nonfatal errors can be ignored if the success percentage is good
        if (error->isFatal() | (this->failureThreshold <= tracker.getRollingSuccessAverage()))
        {
            std::system((this->commandString + " "  + std::to_string(error->getErrorNumber()) + " \"" + std::string(error->what()) + "\"").c_str());
        }
    }
    
    if (error->isFatal())
    {
        std::exit(error->getErrorNumber());
    }
}
