#pragma once
#include "SocketException.h"

class TTLExceededException : public SocketException
{
public:
    TTLExceededException(int ttlTried);
    int getAttemptedTTL();
    
private:
    int ttlTried;
};
