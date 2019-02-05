#ifndef TRACEDISTANCES_H
#define TRACEDISTANCES_H

#include <string>
#include<vector>
#include<map>

#include<sstream>
#include<fstream>

#include "Affordance.h"
#include "DetectedObject.h"



class TraceDistances
{
public:
    TraceDistances(std::string traceLoc,std::vector<std::string> classes,std::pair<int,int> screen);
    ~TraceDistances();
    void addHandDist(std::string classe,double distance, char hand);
    void dumpBuffer();

private:
    std::map<std::string,std::stringstream> buffer;
    std::string fileLoc;
    std::map<std::string,std::string> trace;

    std::pair<int,int> screenSize;
    std::string last;

};

#endif // TRACEDISTANCES_H
