#pragma once
#include "AbstractException.h"
#include <string>

class SocketException : public AbstractException {
    
public:
    SocketException(std::string errorDescription, int errorCode);
    virtual const char* what() const throw();
    
private:
    std::string errorDescription;
};
