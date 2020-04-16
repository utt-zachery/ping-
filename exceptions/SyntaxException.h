#pragma once
#include "AbstractException.h"
#include <string>

class SyntaxException : public AbstractException{
    
public:
    SyntaxException(std::string failedFlag);
    virtual const char* what() const throw();
    
private:
    std::string failedFlag;
    
};
