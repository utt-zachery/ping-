#include "SyntaxException.h"

SyntaxException::SyntaxException(std::string failedFlag) : AbstractException(2, true)
{
    this->failedFlag=failedFlag;
}

const char* SyntaxException::what() const throw()
{
    return ("Incorrect syntax near " + this->failedFlag + ". Try -help for mire information.").c_str();
}
