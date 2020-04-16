#include <stdio.h>
#include "AbstractException.h"

AbstractException::AbstractException(int errorNumber, bool isFatal)
{
    this->errorNumber = errorNumber;
    this->errorIsFatal = isFatal;
}

int AbstractException::getErrorNumber()
{
    return this->errorNumber;
}

bool AbstractException::isFatal()
{
    return this->errorIsFatal;
}
