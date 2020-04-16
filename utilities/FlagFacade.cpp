#include "FlagFacade.h"

FlagFacade::FlagFacade(int argc, const char** arg)
{
    this->tracker = new RollingSuccessTracker();
    this->errorHandler = new ErrorHandlerFlag();
    this->ipAbstraction = new IpFlag();
    this->ttlAbsraction = new TTLFlag();
    this->timeoutAbstraction = new TimeoutFlag();
    this->packetAbstraction = new PacketCountFlag();
    this->minTtlAbstraction = new MinTtlFlag();
    this->waitAbstraction = new WaitFlag();
    this->sampleAbstraction = new SampleFlag();
    
    flags.push_back(this->errorHandler);
    flags.push_back(this->ipAbstraction);
    flags.push_back(this->ttlAbsraction);
    flags.push_back(this->timeoutAbstraction);
    flags.push_back(this->packetAbstraction);
    flags.push_back(this->minTtlAbstraction);
    flags.push_back(this->waitAbstraction);
    flags.push_back(this->sampleAbstraction);
    
    for (int i=0; i < flags.size(); i++)
    {
        try {
            flags[i]->parseCommand( argc, arg);
        } catch (AbstractException* e1) {
            std::cout << e1->what() << std::endl;
            this->errorHandler->handleError(e1, *this->tracker);
        }
    }
}

bool FlagFacade::performFiniteOperations()
{
    bool didSomething = false;
    for (int i=0; i < flags.size(); i++)
    {
       if (flags[i]->executeCommand(this))
       {
           didSomething = true;
       }
    }
    return didSomething;
}

FlagFacade::~FlagFacade()
{
    for (int i=0; i < flags.size(); i++)
    {
        delete flags[i];
    }
}

//Adding interface support to FlagFacade to include each Flag's interface

    //IpFlag Getters
        std::string FlagFacade:: getIPAddress() {
            return this->ipAbstraction->getIPAddress();
        }
        struct hostent* FlagFacade:: getActiveHost() {
            return this->ipAbstraction->getActiveHost();
        }

        struct in_addr FlagFacade:: getIPv4Addr() {
            return this->ipAbstraction->getIPv4Addr();
        }

        struct in6_addr FlagFacade:: getIPv6Addr() {
            return this->ipAbstraction->getIPv6Addr();
        }

        bool FlagFacade::isIPv4() {
            return this->ipAbstraction->isIPv4();
        }

        bool FlagFacade::isIPv6() {
            return this->ipAbstraction->isIPv6();
        }

    //ErrorHandler methods
        void FlagFacade::handleError(AbstractException* error) {
            return this->errorHandler->handleError(error, *this->tracker);
        }

    //TTLFlag methods
        int FlagFacade:: getTTL() {
            return this->ttlAbsraction->getTTL();
        }
        void FlagFacade::setTTL(int toSet)
        {
            this->ttlAbsraction->setTTL(toSet);
        }


    //TimeoutFlag Methods
        int FlagFacade:: getTimeout() {
            return this->timeoutAbstraction->getTimeout();
        }

        void FlagFacade::addSuccess() {
            this->tracker->addSuccess();
        }

        void FlagFacade::addFailure() {
            this->tracker->addFailure();
        }

        double FlagFacade::getRollingAverage() {
            return this->tracker->getRollingSuccessAverage();
        }

    //WaitFlag
        int FlagFacade::getWaitTime() {
            return this->waitAbstraction->getWaitTime();
        }


    //PacketCountFlag Methods
        int FlagFacade::getPacketCount() {
            return this->packetAbstraction->getPacketCount();
        }
