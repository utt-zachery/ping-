#include "MinTtlFlag.h"

MinTtlFlag::MinTtlFlag()
{
    this->isActive = false;
}

void MinTtlFlag::parseCommand(int argc, const char ** arg)
{
    for (int i=1; i < argc; i++)
    {
        if (std::string(arg[i]) == "-m")
        {
            this->isActive = true;
        }
    }
}

bool MinTtlFlag::executeCommand(void* flagFacade)
{
    if (this->isActive)
    {
        try {
            FlagFacade* facade = (FlagFacade*)flagFacade;
            int originalTTL = facade->getTTL();
            std::cout<<"------------------------------ Minimum TTL ------------------------------ "<<std::endl;
            PingCommand pinger(facade);
            facade->setTTL(1);
            
            int counter = 0;
            while (true)
            {
                counter++;
                facade->setTTL(counter);
                
                try {
                    bool completed = pinger.executePing();
                    if (completed) {
                        break;
                    }
                } catch (std::exception* q)
                {
                    counter=counter+1;
                    std::this_thread::sleep_for (std::chrono::milliseconds(100));
                    continue;
                } 
                
            }
    
                facade->setTTL(originalTTL);
            std::cout << "\nMinimum TTL Needed: " <<counter<<std::endl;
        } catch (std::exception e) {
            throw new std::runtime_error("Argument must be a FlagFacade*");
        }
        return true;
    }
    return false;
}
