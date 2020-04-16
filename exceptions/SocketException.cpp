
#include "SocketException.h"

SocketException::SocketException(std::string failedDescription, int errorCode) : AbstractException(errorCode, false)
{
    this->errorDescription=failedDescription;;
}

const char* SocketException::what() const throw()
{
    return ("\nSocket Exception: " + this->errorDescription).c_str();
}
