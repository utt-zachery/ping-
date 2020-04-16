#pragma once
//Establishes an abstraction for all flags, allowing for quick parsing

class AbstractFlag {

public:
    
    //Each flag's default operation will be defined in the constructor
    
    //Override deafult operation and do the user requested operation instead
    virtual void parseCommand(int argc, const char** argv) = 0;
    
    virtual bool executeCommand(void* flagFacade) {
        return false;
    }
    
    virtual ~AbstractFlag()
    {
        
    };
};
