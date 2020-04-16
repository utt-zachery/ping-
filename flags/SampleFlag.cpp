#include "SampleFlag.h"

SampleFlag::SampleFlag()
{
    this->sampleSize = 40;
    this->isActive = false;
}

bool SampleFlag::executeCommand(void* flagFacade)
{
    if (this->isActive)
    {
        try {
            FlagFacade* facade = (FlagFacade*)flagFacade;
            std::cout<<"------------------------------ Sample Statistics ------------------------------ "<<std::endl;
            
            PingCommand pinger(facade);
            
            for (int i=0; i < this->sampleSize; i++)
            {
                pinger.doPing();
                std::this_thread::sleep_for (std::chrono::milliseconds(facade->getWaitTime()));
            }
            
            std::cout<< std::endl;
            std::cout<<"------------------------------ Summary Statistics ------------------------------ "<<std::endl<<std::endl;
            double average = pinger.getAverage();
            double variance =pinger.getVariance();
            
            double phat = facade->getRollingAverage();
            
            std::cout<<"n (amount of trials): " << this->sampleSize <<std::endl;
            std::cout<<"p̂ (proportion of successful packet trials): " << phat <<std::endl;
            std::cout<<"x̄ (average wait time in miliseconds): " << average <<std::endl;
            std::cout<<"σ² (variance of wait time in miliseconds): " << variance <<std::endl;
            std::cout<<std::endl;
            std::cout<<"------------------------------ Confidence Intervals ------------------------------ "<<std::endl;
            std::cout<<std::endl<<"90% Confidence Interval for p: (" << (phat - 1.64 * sqrt(phat*(1-phat) /this->sampleSize ))<<","<< (phat + 1.64 * sqrt(phat*(1-phat) /this->sampleSize ))<<")" <<std::endl;
            
            std::cout<<"95% Confidence Interval for p: (" << (phat - 1.96 * sqrt(phat*(1-phat) /this->sampleSize ))<<","<< (phat + 1.96 * sqrt(phat*(1-phat) /this->sampleSize ))<<")" <<std::endl;
            
            std::cout<<"99% Confidence Interval for p: (" << (phat - 2.58 * sqrt(phat*(1-phat) /this->sampleSize ))<<","<< (phat + 2.58 * sqrt(phat*(1-phat) /this->sampleSize ))<<")" <<std::endl;
            
            std::cout<<"90% Confidence Interval for μ: (" << (average - 1.64 * sqrt(variance))<<","<< (average + 1.64 * sqrt(variance))<<")" <<std::endl;
            
            std::cout<<"95% Confidence Interval for μ: (" << (average - 1.96 * sqrt(variance))<<","<< (average + 1.96 * sqrt(variance))<<")" <<std::endl;
            
            std::cout<<"99% Confidence Interval for μ: (" << (average - 2.58 * sqrt(variance))<<","<< (average + 2.58 * sqrt(variance))<<")" <<std::endl;
            
            std::cout<<std::endl<<"------------------------------ Probabilities ------------------------------ "<<std::endl<<std::endl;
           
            double p = facade->getRollingAverage();
            p = 0.8;
            double granularity = 200;
            for (int i=0; i <= granularity; i=i+granularity/20)
            {
                if (p!= 1.0 && p!=0)
                {
                    long double probability = 0.0;
                    for (int a=0; a<= i; a++)
                    {
                        probability=probability + ((tgamma((long double)( granularity + 1)) / (tgamma((long double)(a+1)) * (tgamma( (long double) ((granularity - a +1)  ) )))) * (long double)pow((long double)p, (long double)a) * (long double)pow((long double)(1-p), (long double)(granularity - a)));
                    }
                    if (isnan(probability))
                        probability=1;
                    std::cout<<"Probability that " << (int)(100*(double)i/granularity)<< "% or fewer packets will be returned: " << (probability) <<std::endl;
                }
                else if (p==1)
                {
                     std::cout<<"Probability that " << (int)(100*(double)i/granularity) << "% or fewer packets will be returned: 1" <<std::endl;
                }
                else if (p==0)
                {
                    std::cout<<"Probability that " <<  (int)(100*(double)i/granularity) << "% or fewer packets will be returned: 0" <<std::endl;
                }
            }
            
        } catch (std::exception e) {
            throw new std::runtime_error("Argument must be a FlagFacade*");
        }
        return true;
    }
    return false;
}

void SampleFlag::parseCommand(int argc, const char ** arg)
{
    for (int i=1; i < argc; i++)
    {
        if (std::string(arg[i]) == "-s")
        {
            this->isActive = true;
            if (i+1<argc && this->isNumber(std::string(arg[i+1])))
            {
                this->sampleSize = std::stoi(std::string(arg[i+1]));
            }
        }
    }
}

bool SampleFlag::isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

