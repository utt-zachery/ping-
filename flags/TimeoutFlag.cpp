#include "TimeoutFlag.h"

TimeoutFlag::TimeoutFlag()
{
    this->timeout = 60;
}

int TimeoutFlag::getTimeout()
{
    return this->timeout;
}

bool TimeoutFlag::isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void TimeoutFlag::parseCommand(int argc, const char ** arg)
{
    for (int i=1; i < argc ; i++)
    {
        if (std::string(arg[i]) == "-o")
        {
            if (!isNumber(std::string(arg[i+1])))
            {
                throw new SyntaxException("\"" + std::string(arg[i+1]) + "\" (positive integer  expected)");
            }
            this->timeout = std::stoi(arg[i+1]);
        }
    }
}
