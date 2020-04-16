#pragma once
#include <list>
#include <string>
#include <iostream>

//Singleton Success Tracker that uses a rolling average to trigger an ErrorHandling flag if the current success percentage drops below a threshold

class RollingSuccessTracker {
    
public:
    void addSuccess();
    void addFailure();
    double getRollingSuccessAverage();
    bool largeEnoughSample();
    RollingSuccessTracker();
    
private:
    
    //Helper variables so we will not have to iterate over the entire history every time
    int successes;
    int failures;
    
    //History linked list that will hold only the most recent 100 values to track changes in PING success, in constant time
    std::list<bool> history;
    
    void popOf();
    
    std::string formatRollingAveragePercentage();
};
