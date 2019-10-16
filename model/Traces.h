#ifndef TRACE_H
#define TRACE_H

#include <string>
#include<vector>
#include<sstream>
#include<fstream>

#include "recoActivite/recoAffordance/Affordance.h"

class Traces
{
public:
    Traces(std::string traceLoc,std::string planName);
    ~Traces();
    inline bool exists_test0 (const std::string& name);

    void addAffordance(Affordance* aff);
    void addFutureActivities(std::string ac1);
    void addCurrentPlan(std::string plan);
    void addPossiblePlans(std::string plan);
    void addPlanReset();
    void dumpBuffer();
private:
    std::stringstream buffer;
    std::string fileLoc;

    std::ofstream trace;
    std::string last;

};

#endif // TRACE_H
