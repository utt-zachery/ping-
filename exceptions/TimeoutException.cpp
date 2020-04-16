#include "TimeoutException.h"

TimeoutException::TimeoutException(int timeOut) : SocketException("Connection timed out after " + std::to_string(timeOut) + " seconds", 12)
{
    this->timeOut = timeOut;
}

int TimeoutException::getTimeout()
{
    return this->timeOut;
}
