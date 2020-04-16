#include "TTLExceededException.h"

TTLExceededException::TTLExceededException(int ttlTried) : SocketException("TTL of " + std::to_string(ttlTried)+" was too low", 11)
{
    this->ttlTried = ttlTried;
}

int TTLExceededException::getAttemptedTTL()
{
    return this->ttlTried;
}
