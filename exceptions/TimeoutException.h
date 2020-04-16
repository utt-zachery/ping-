#pragma once
#include "SocketException.h"

class TimeoutException : public SocketException
{
public:
    TimeoutException(int timeOut);
    int getTimeout();
    
private:
    int timeOut;
};
