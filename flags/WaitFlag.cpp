#include "WaitFlag.h"

WaitFlag::WaitFlag()
{
    this->customTTL = 5000;
}

int WaitFlag::getWaitTime()
{
    return this->customTTL;
}


bool WaitFlag::isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void WaitFlag::parseCommand(int argc, const char ** arg)
{
    for (int i=1; i < argc; i++)
    {
        if (std::string(arg[i]) == "-w")
        {
            if (!isNumber(std::string(arg[i+1])))
            {
                throw new SyntaxException("\"" + std::string(arg[i+1]) + "\" (positive integer  expected)");
            }
            this->customTTL = std::stoi(arg[i+1]);
        }
    }
}

