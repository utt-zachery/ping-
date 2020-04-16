#include "PingCommand.h"

#define DEFDATALEN (64-ICMP_MINLEN)
#define MAXIPLEN 60
#define MAXICMPLEN 76
#define MAXPACKET (65536 - 60 - ICMP_MINLEN)

PingCommand::PingCommand(FlagFacade* facade)
{
    this->facade = facade;
    std::cout << facade->getActiveHost()->h_name<< " (" << facade->getIPAddress()<<")"<< std::endl;
    if (facade->isIPv4())
    {
        std::cout << "Protocol: IPv4" << std::endl;
    }
    else if (facade->isIPv6())
    {
        std::cout << "Protocol: IPv6" << std::endl;;
    }
}

double PingCommand::getAverage()
{
    double total = 0;
    
    for (auto iterator = this->timeStamps.begin(); iterator != this->timeStamps.end(); iterator++)
    {
        total = total + *iterator;
    }
    
    double average = total / (double) this->timeStamps.size();
    return average;
}

double PingCommand::getVariance()
{
    double average = this->getAverage();
    double total = 0;
    
    for (auto iterator = this->timeStamps.begin(); iterator != this->timeStamps.end(); iterator++)
    {
        total = total + (average - *iterator)*(average - *iterator);
    }
    
    double var = total / (double) this->timeStamps.size();
    return var;
}

void PingCommand::clear()
{
    this->timeStamps.clear();
}

void PingCommand::addTime(int currentTime)
{
    this->timeStamps.push_back(currentTime);
}

bool PingCommand::doPing() {
    try {
         return this->executePing();
    } catch (AbstractException* e1) {
        std::cout << e1->what() << std::endl;
        facade->handleError(e1);
        return false;
    }
}

bool PingCommand::executePing() throw(AbstractException*)
{
    //Core socket handle
    int s = 0;
    
    //Set content of package to max
    int cc, packlen, datalen = DEFDATALEN;
    
    // IPv4 helper variables
    struct sockaddr_in to, from;
  
    //IPv6 helper variables
    struct sockaddr_in6 to6;
    
    //Packet data structure
    u_char *packet, outpack[MAXPACKET];
    
    struct icmp *icp;
    
    //Socket Handle
    fd_set rfds;
    
    //Load Time components now so we do not waste time spent measuring response time on creating objects
    
         std::vector<struct timeval> startTimes;
         for (int i=0 ; i < this->facade->getPacketCount(); i++)
         {
             struct timeval start;
             startTimes.push_back(start);
         }
    
        struct timeval end;
        long end_t;
    
    
    //Confgure socket address, if IPv4
    if (this->facade->isIPv4()) {
        to.sin_family = this->facade->getActiveHost()->h_addrtype;
        to.sin_addr = this->facade->getIPv4Addr();
    } else if (this->facade->isIPv6())
    {
        to6.sin6_family = this->facade->getActiveHost()->h_addrtype;
        to6.sin6_addr = this->facade->getIPv6Addr();
    }
    
    packlen = datalen + MAXIPLEN + MAXICMPLEN;
    
    int ttl = this->facade->getTTL();
    
    if (this->facade->isIPv4()) {
        if ( (s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
            throw new UserPermissionsException();
            return false;
        }
        
        
        
    }else if (this->facade->isIPv6()) {
        if ( (s = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMP)) < 0) {
            throw new UserPermissionsException();
            return false;
        }
    }
    
    setsockopt(s, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
    
    std::map<int, int> packetSequenceStateChecker;
    //
    //  First int represents the unique packet sequence ID
    //  Second int represents the state of the packet
    //  1 = sent
    //  2 = received
    //
    // This map will allows us to track if packet sequence IDs are correct and not duplicated
    
    for (int i=0 ; i < this->facade->getPacketCount(); i++)
    {
        //Prepare packet header
        icp = (struct icmp *) outpack;
        icp->icmp_type = ICMP_ECHO;
        icp->icmp_code = 0;
        icp->icmp_cksum = 0;
        icp->icmp_seq = 10000+i; //Match up an individual packet with its response
        packetSequenceStateChecker.emplace(10000+i,1);
        icp->icmp_id = getpid();
        
        cc = datalen + ICMP_MINLEN;
        icp->icmp_cksum = checksum((unsigned short *)icp,cc);
        
        gettimeofday(&startTimes[i], NULL);
        
        //Send the package over the socket
        long bytesSent = 0L;
        if (this->facade->isIPv4()) {
            bytesSent = sendto(s, (char *)outpack, cc, 0, (struct sockaddr*)&to, (socklen_t)sizeof(struct sockaddr_in));
        }else if (this->facade->isIPv6()) {
            bytesSent = sendto(s, (char *)outpack, cc, 0, (struct sockaddr*)&to6, (socklen_t)sizeof(struct sockaddr_in6));
        }
        
        //Check if failbit is true, or if not all bytes were sent
        
        if (bytesSent < 0)
        {
            this->facade->addFailure();
            throw new SocketException("Failed to send bytes!", 5);
            return false;
        }
        if (bytesSent != cc)
        {
            this->facade->addFailure();
            throw new SocketException("Failed to send all of the bytes in the packet!", 5);
            return false;
        }
        
        
        if (i==0)
        {
            std::cout << "Sent " << this->facade->getIPAddress() << " packet (ID: 1) " <<  bytesSent<< " bytes";
        } else {
            std::cout << "\nSent " << this->facade->getIPAddress() << " packet (ID: " << std::to_string(i+1)<< ") " <<  bytesSent<< " bytes";
        }
        
          
            
    }
    
    for (int i=0 ; i < this->facade->getPacketCount(); i++)
    {
            FD_SET(s, &rfds);
        
            struct timeval timeOut;
            timeOut.tv_sec =this->facade->getTimeout();
            
            int connectionIndicator = select(s+1, &rfds, NULL, NULL, &timeOut);
        
            if (connectionIndicator == -1)
            {
                this->facade->addFailure();
                   close(s);
                throw new SocketException("Failed to read bytes!", 6);
                return false;
            } else if (connectionIndicator == 0 ) {
                   close(s);
                throw new TimeoutException(this->facade->getTimeout());
                return false;
            }  else if (connectionIndicator > 0) {
                
                // Allocate memory for packet
                if ( (packet = (u_char *)malloc((u_int)packlen)) == NULL)
                {
                       close(s);
                    throw new SocketException("Memory Allocation Exception", 4);
                    return false;
                }
                
                long bytesRead = 0L;
                int fromAddressLength = sizeof(sockaddr_in);
                
                if ( (bytesRead = recvfrom(s, (char *)packet, packlen, 0,(struct sockaddr *)&from, (socklen_t*)&fromAddressLength)) < 0)
                {
                       close(s);
                    this->facade->addFailure();
                    throw new SocketException("Failed to read bytes!", 6);
                    return false;
                }
                
                
                // Check header
                int headerLength = sizeof( struct ip );
                if (bytesRead < (headerLength + ICMP_MINLEN))
                {
                       close(s);
                    this->facade->addFailure();
                    throw new SocketException("Packet received was critically short", 7);
                    return false;
                }
                
                // Deconstruct ICMP Header
                icp = (struct icmp *)(packet + headerLength);
                if (icp->icmp_type == ICMP_ECHOREPLY)
                {
                    int seq = icp->icmp_seq;
                    if (seq < 10000 || seq > this->facade->getPacketCount()+9999)
                    {
                           close(s);
                        this->facade->addFailure();
                        throw new SocketException("Packet received had the incorrect sequence", 8);
                        return false;
                    }
                    std::map<int,int>::iterator it = packetSequenceStateChecker.find(seq);
                    if (it == packetSequenceStateChecker.end())
                    {
                        close(s);
                        this->facade->addFailure();
                        throw new SocketException("Packet received had the incorrect sequence", 8);
                        return false;
                    }
                    else if (it->second != 1)
                    {
                        close(s);
                        this->facade->addFailure();
                        throw new SocketException("Packet received had the incorrect sequence", 8);
                        return false;
                    }
                    
                    packetSequenceStateChecker[seq] = 2;
                    
                    if (icp->icmp_id != getpid())
                    {
                           close(s);
                        this->facade->addFailure();
                        throw new SocketException("Packet received was not a response to the packet sent", 9);
                        return false;
                    }
                }
                else if (icp->icmp_type == ICMP_TIMXCEED)
                {
                       close(s);
                    this->facade->addFailure();
                    throw new TTLExceededException(this->facade->getTTL());
                    return false;
                }
                else
                {
                       close(s);
                    this->facade->addFailure();
                    throw new SocketException("Received a packet other than an echo packet", 10);
                    return false;
                }
                
                gettimeofday(&end, NULL);
                end_t = 1000*(end.tv_sec - startTimes[i].tv_sec) + (end.tv_usec - startTimes[i].tv_usec)/1000;
                delete packet;
                
                if(end_t < 1)
                    end_t = 1;
                
                this->addTime((int)end_t);
                
                if (this->facade->getPacketCount() == 1)
                {
                    std::cout << ", received " << bytesRead << " bytes" <<std::endl;
                    std::cout << "\tElapsed time: " << end_t << " miliseconds" << std::endl;
                } else {
                    if (i==0) {
                        std::cout << "\nReceived packet (ID: 1) containing " << bytesRead << " bytes from "<< this->facade->getIPAddress() <<std::endl;
                    } else {
                        std::cout << "Received packet (ID: "<< icp->icmp_seq -9999<<") containing " << bytesRead << " bytes from "<< this->facade->getIPAddress() <<std::endl;
                    }
                    std::cout << "\tElapsed time: " << end_t << " miliseconds" << std::endl;
                   
                }
                this->facade->addSuccess();
            }
    }
 
    return true;
}

unsigned short PingCommand::checksum(void *b, int len)
{
    unsigned short* buffer = (unsigned short*)b;
    unsigned int sum=0;
    unsigned short result;
    
    for ( sum = 0; len > 1; len -= 2 )
    {
        sum += *buffer++;
    }
    if ( len == 1 )
    {
        sum += *(unsigned char*)buffer;
    }
    
    //Bitshift packets
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    
    //Perform bit inverse
    result = ~sum;
    return result;
}
