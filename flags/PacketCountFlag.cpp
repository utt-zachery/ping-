#include "PacketCountFlag.h"

PacketCountFlag::PacketCountFlag()
{
    this->packetCount = 1;
}

void PacketCountFlag::parseCommand(int argc, const char ** arg)
{
    for (int i=1; i < argc; i++)
    {
        if (std::string(arg[i]) == "-c")
        {
            if (!isNumber(std::string(arg[i+1])))
            {
                throw new SyntaxException("\"" + std::string(arg[i+1]) + "\" (positive integer  expected)");
            }
            this->packetCount = std::stoi(arg[i+1]);
        }
    }
}

int PacketCountFlag::getPacketCount()
{
    return this->packetCount;
}

bool PacketCountFlag::isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
