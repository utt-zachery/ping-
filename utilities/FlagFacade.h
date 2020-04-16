#pragma once
#include "../flags/IpFlag.h"
#include "../flags/ErrorHandlerFlag.h"
#include "../flags/TTLFlag.h"
#include "../flags/TimeoutFlag.h"
#include "../flags/PacketCountFlag.h"
#include "../flags/MinTtlFlag.h"
#include "../flags/WaitFlag.h"
#include "../flags/SampleFlag.h"
#include "RollingSuccessTracker.h"
#include <iostream>

class MinTtlFlag;
class SampleFlag;

class FlagFacade
{
    public:
        FlagFacade(int argc, const char** arg);
        ~FlagFacade();
        bool performFiniteOperations();
    
    
        //Adding interface support to FlagFacade to include each Flag's interface
    
            //IpFlag Getters
                    std::string getIPAddress();
                    struct hostent* getActiveHost();
                    struct in_addr getIPv4Addr();
                    struct in6_addr getIPv6Addr();
                    bool isIPv4();
                    bool isIPv6();
    
            //ErrorHandler methods
                    void handleError(AbstractException* error);
    
            //TTLFlag methods
                    int getTTL();
                    void setTTL(int toSet);
    
            //TimeoutFlag Methods
                    int getTimeout();
    
            //PacketCountFlag Methods
                     int getPacketCount();
    
            //WaitFlag method
                    int getWaitTime();
    
            //Tracker Methods
                    void addSuccess();
                    void addFailure();
                    double getRollingAverage();
    
    private:
        IpFlag* ipAbstraction;
        RollingSuccessTracker* tracker;
        ErrorHandlerFlag* errorHandler;
        TTLFlag* ttlAbsraction;
        TimeoutFlag* timeoutAbstraction;
        PacketCountFlag* packetAbstraction;
        MinTtlFlag* minTtlAbstraction;
        WaitFlag* waitAbstraction;
        SampleFlag* sampleAbstraction;
    
        std::vector<AbstractFlag*> flags;
};
