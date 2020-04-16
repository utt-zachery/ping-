#pragma once
#include "../exceptions/UserPermissionsException.h"
#include "../exceptions/AbstractException.h"
#include "../exceptions/SocketException.h"
#include "../exceptions/TTLExceededException.h"
#include "../exceptions/TimeoutException.h"
#include "FlagFacade.h"
#include <vector>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <list>
#include <map>

class FlagFacade;
class PingCommand {
public:
    PingCommand(FlagFacade* facade);
    bool executePing() throw(AbstractException*); //Use this method if you want to handle excpetions yourself
    bool doPing(); // Use this method if you want exceptions to be handled for you using the active ExceptionHandler
    void clear();
    double getAverage();
    double getVariance();
    
private:
    unsigned short checksum(void *b, int len);
    FlagFacade* facade;
    std::list<int> timeStamps;
    void addTime(int currentTime);
};
