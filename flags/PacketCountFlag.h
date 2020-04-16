#pragma once
#include "AbstractFlag.h"
#include "SyntaxException.h"
#include <string>

class PacketCountFlag : public AbstractFlag
{
public:
    PacketCountFlag();
    
    //Default behavior will be set to 255. If the user passes in a command string and the threshold is reached, the command will be executed
    void parseCommand(int argc, const char** arg);
    
    int getPacketCount();
    
private:
    int packetCount;
    bool isNumber(const std::string& s);
};
