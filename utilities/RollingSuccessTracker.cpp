#include "RollingSuccessTracker.h"

// Success Tracker class that uses a rolling average to trigger an ErrorHandling flag if the current success percentage drops below a threshold

RollingSuccessTracker::RollingSuccessTracker() {
    this->successes=0;
    this->failures=0;
}

bool RollingSuccessTracker::largeEnoughSample()
{
    return (this->history.size() > 50);
}

double RollingSuccessTracker::getRollingSuccessAverage() {
    return (double)this->successes / (double)(this->successes + this->failures);
}

std::string RollingSuccessTracker::formatRollingAveragePercentage()
{
    return std::to_string((int)(100*this->getRollingSuccessAverage())) + "%";
}

void RollingSuccessTracker::popOf() {
    
    if (this->history.size() > 100) {
        if (this->history.back()) {
            successes--;
        }
        else {
            failures--;
        }
        
        this->history.pop_back();
    }
}

void RollingSuccessTracker::addSuccess() {
    this->successes=this->successes+1;
    this->history.push_back(true);
    this->popOf();
    std::cout << "\tRolling Success Rate: " << this->formatRollingAveragePercentage()<< std::endl;
}

void RollingSuccessTracker::addFailure() {
    this->failures=this->failures+1;
    this->history.push_back(false);
    this->popOf();
      std::cout << "\tRolling Success Rate: " << this->formatRollingAveragePercentage()<< std::endl;
}

