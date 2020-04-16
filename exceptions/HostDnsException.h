#pragma once
#include "AbstractException.h"
#include <string>

class HostDnsException : public AbstractException{
    
public:
    HostDnsException(std::string failedDNS);
    virtual const char* what() const throw();
    
    
private:
    std::string failedDNS;
    
};
