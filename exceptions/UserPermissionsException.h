#pragma once
#include "AbstractException.h"
#include <string>

class UserPermissionsException : public AbstractException {
    
public:
    UserPermissionsException();
    virtual const char* what() const throw();
    
};
