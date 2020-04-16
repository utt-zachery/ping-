#pragma once
#include "AbstractFlag.h"
#include "../exceptions/AbstractException.h"
#include <string>
#include "sys/socket.h"
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <regex>
#include "../exceptions/HostDnsException.h"
#include "../exceptions/SyntaxException.h"


//Concrete flag class that represents the name of the domain / IP address to connect to
class IpFlag : public AbstractFlag {
    
public:
    //Override function that replaces default behavior with user's custom
    virtual void parseCommand(int argc, const char** argv);
    
    //Getter method for when the PING command is actually executed
    std::string getIPAddress();
    
    IpFlag();
    struct hostent* getActiveHost();
    struct in_addr getIPv4Addr();
    struct in6_addr getIPv6Addr();
    bool isIPv4();
    bool isIPv6();
    
    
private:
    std::string ipAddress;
    struct hostent* activeHost;
    
    bool IpV4;
    bool IpV6;
    
    //Helper method to convert a DNS address to its corresponding IP Address
    void resolveDNStoIP(std::string dnsAddress, bool isIP) throw(AbstractException*);
    
   
};
