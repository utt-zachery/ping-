#include "HostDnsException.h"

HostDnsException::HostDnsException(std::string failedDNS) : AbstractException(1, true)
{
    this->failedDNS=failedDNS;
}

const char* HostDnsException::what() const throw()
{
     return ("The DNS lookup on " + this->failedDNS + " failed. This could be because of an invalid DNS, or because of an inactive internet connection.").c_str();
}
