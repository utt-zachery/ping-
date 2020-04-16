#pragma once
#include <exception>

class AbstractException : public std::exception {
    
public:
    virtual const char* what() const throw() = 0;
    int getErrorNumber();
    AbstractException(int errorNumber, bool isFatal);
    bool isFatal();
    
private:
    int errorNumber;
    bool errorIsFatal;
};

