#ifndef TRACEMANAGER_H
#define TRACEMANAGER_H


#include <string>
#include<vector>
#include<sstream>
#include<fstream>

#include "Affordance.h"

class TraceManager
{
public:
    TraceManager(std::string traceLoc,std::pair<int,int> screen);
    ~TraceManager();
    void addAffordance(AffordanceTime* aff,int time);
    void addFutureActivities(std::string ac1,int i,int time);
    void addCurrentPlan(std::string plan,int time);
    void addFrameCount(clock_t time);
    void addTotalCount(clock_t timeStart,clock_t timeEnd);

    void dumpBuffer();
private:
    std::stringstream buffer;
    std::string fileLoc;
    std::stringstream bufferTime;
    std::ofstream traceTime;

    std::ofstream trace;
    std::pair<int,int> screenSize;
    std::string last;

};

#endif // TRACEMANAGER_H
