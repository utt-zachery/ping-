#include "TTLFlag.h"

TTLFlag::TTLFlag()
{
    this->customTTL = 255;
}

int TTLFlag::getTTL()
{
    return this->customTTL;
}

void TTLFlag::setTTL(int toSet)
{
    this->customTTL = toSet;
}

bool TTLFlag::isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void TTLFlag::parseCommand(int argc, const char ** arg)
{
    for (int i=1; i < argc; i++)
    {
       if (std::string(arg[i]) == "-t")
        {
            if (!isNumber(std::string(arg[i+1])))
            {
                throw new SyntaxException("\"" + std::string(arg[i+1]) + "\" (positive integer  expected)");
            }
            this->customTTL = std::stoi(arg[i+1]);
        }
    }
}
