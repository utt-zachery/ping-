#include "UserPermissionsException.h"

UserPermissionsException::UserPermissionsException() : AbstractException(3, true)
{
    
}

const char* UserPermissionsException::what() const throw()
{
    return "Inadequate permissions. Please rerun as root user";
}

