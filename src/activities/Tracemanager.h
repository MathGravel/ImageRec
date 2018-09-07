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
    void dumpBuffer();
private:
    std::stringstream buffer;
    std::string fileLoc;
    std::ofstream trace;
    std::pair<int,int> screenSize;
    std::string last;
};

#endif // TRACEMANAGER_H
