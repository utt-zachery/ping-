#include "IpFlag.h"

//Constructor that represents default behavior if the user does not specifiy this flag in the command line
IpFlag::IpFlag()
{
    this->IpV4=false;
    this->IpV6=false;
    this->activeHost = nullptr;
}

//Inherited parsing function to alter the behavior of this object based on user input
void IpFlag:: parseCommand(int argc, const char * argv[])
{
    std::regex url_regex ("^(https?:\/\/)?([\da-z\.-]+)\.([a-z\.]{2,6})([\/\w \.-]*)*\/?$");
    std::smatch url_match_result;
    std::regex regexIpV6ORipV4 ("(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))");
    
    std::string input = std::string(argv[1]);
    bool isUrl = std::regex_search(input, url_regex);
    bool isIP =std::regex_search(input, regexIpV6ORipV4);
    if (argc>1)
    {
        if (!isUrl && !isIP)
        {
            throw new SyntaxException("argument 1 (hostname / IP address)");
        }
        if (!isIP)
            this->resolveDNStoIP(std::string(argv[1]), false);
        else
        {
            this->resolveDNStoIP(std::string(argv[1]), true);
            
        }
    }
    else
    {
        resolveDNStoIP("http://google.com/", false);
    }
}

struct hostent* IpFlag::getActiveHost()
{
    return this->activeHost;
}

struct in_addr IpFlag::getIPv4Addr()
{
     if (this->activeHost == nullptr) throw(10);
     return *((struct in_addr **)this->activeHost->h_addr_list)[0];
}

struct in6_addr IpFlag::getIPv6Addr()
{
    if (this->activeHost == nullptr) throw(10);
    
    return *((struct in6_addr **)this->activeHost->h_addr_list)[0];
}

void IpFlag::resolveDNStoIP(std::string dnsAddress, bool isIP) throw(AbstractException*)
{
    if (dnsAddress.find("http") !=std::string::npos)
    {
        dnsAddress = dnsAddress.substr(2+dnsAddress.find("//"));
    }
    
    if (dnsAddress.find_last_of("/") == dnsAddress.length()-1)
    {
        dnsAddress = dnsAddress.substr(0,dnsAddress.length()-1);
    }
    
    if (!isIP) {
        this->activeHost = gethostbyname(dnsAddress.c_str());
    } else {
        this->activeHost = gethostbyaddr(dnsAddress.c_str(), (int)(sizeof(char) * dnsAddress.size()), AF_INET);
    }
    
    if (this->activeHost == nullptr)
    {
        throw new HostDnsException(dnsAddress);
    }
    if (this->activeHost->h_addrtype == AF_INET)
    {
        this->IpV6=false;
        this->IpV4=true;
        this->ipAddress = inet_ntoa(this->getIPv4Addr());
    }
    else if (this->activeHost->h_addrtype == AF_INET6)
    {
        this->IpV6=true;
        this->IpV4=false;
        
        char output[INET6_ADDRSTRLEN];
        in6_addr qptr = this->getIPv6Addr();
        inet_ntop(AF_INET6, (const void *)&qptr, output, INET6_ADDRSTRLEN);
        this->ipAddress = std::string(output);
    }
}

std::string IpFlag::getIPAddress()
{
    return this->ipAddress;
}

bool IpFlag::isIPv4()
{
    return this->IpV4;
}

bool IpFlag::isIPv6()
{
    return this->IpV6;
}

