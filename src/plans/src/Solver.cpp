#include <vector>
#include <iostream>
#include <map>
#include <utility>
#include <string>
#include "../include/Generator.h"
#include "../include/Domain.h"
#include "Solver.h"


Solver::Solver()
{
    this->domain = Domain();
    this->depth = 0;
    this->siz = 0;
    this->currentObservation = std::pair<int, int>();
    this->generators = std::map<int, std::vector<Output>>();
    this->timeMap = std::map<int, std::vector<std::pair<float,float>>>();
}

Solver::Solver(Domain domain, int depth, int siz)
{
    this->siz= siz;
    this->domain= domain;
    this->depth = depth;
    this->currentObservation = std::pair<int, int>(0,-1);
    this->generators = std::map<int, std::vector<Output>>();
    this->timeMap = std::map<int, std::vector<std::pair<float,float>>>();
    std::vector<Output> outputs;
    while(outputs.size() != siz)
    {
        outputs.push_back(Output(&this->domain));
    }
    for(std::vector<Output>::iterator it = outputs.begin(); it != outputs.end(); ++it)
    {
        bool ok = true;
        while((it->getPOOutput().size() < this->depth) && ok)
        {
            ok = it->expand();
        }
        if(ok)
        {
        if(this->generators.count(it->getLastActionPO())==0)
        {
            this->generators[it->getLastActionPO()] = std::vector<Output>();
            this->timeMap[it->getLastActionPO()] = std::vector<std::pair<float,float>>();
        }

        this->generators[it->getLastActionPO()].push_back(*it);
        this->timeMap[it->getLastActionPO()].push_back((*it).getTimeDurationAction(0));
        }

    }
}

bool Solver::addObservation (std::string observation)
{
    return this->addObservation(this->domain.getPlanLibrary().getLiteralId(observation));
}

bool Solver::addObservation(int lit)
{

    this->currentObservation = std::pair<int, int>(this->currentObservation.first +1 , lit);
    if(this->generators.count(lit) == 0)
    {
        std::cout << "non accessible observation"<<std::endl;
        return false;
    }
    std::vector<Output> currentOutput = this->generators[lit];
    int actualSize = currentOutput.size();
    //TODO improve selection with time distance;

    while(actualSize < this->siz)
    {
        int randomIndex = rand() % currentOutput.size();
        currentOutput.push_back(currentOutput[randomIndex]);
        actualSize++;
    }

    this->generators.clear();

    for(std::vector<Output>::iterator it = currentOutput.begin(); it != currentOutput.end();++it)
    {
        bool ok = true;
        while((it->getPOOutput().size() < (this->depth + this->currentObservation.first)) && ok)
        {
            ok = it->expand();
        }
        if(ok)
        {
            if(this->generators.count(it->getLastActionPO())==0)
            {
                this->generators[it->getLastActionPO()]; //= std::vector<Output>();
                this->timeMap[it->getLastActionPO()];//] = std::vector<std::pair<float,float>>();
            }
            this->generators[it->getLastActionPO()].push_back(*it);
            this->timeMap[it->getLastActionPO()].push_back((*it).getTimeDurationAction(0));
        }
    }
    return true;
}

const std::map<std::string, float> Solver::getNextActions(int depth)const
{
    std::map<std::string, float> out = std::map<std::string, float>();
    for(std::map<int, std::vector<Output>>::const_iterator it = this->generators.begin(); it!= this->generators.end(); ++it )
    {
        out[this->domain.getPlanLibrary().getLiteralName(it->first)] = 100*((float)it->second.size()/(float)this->siz);
    }
    return out;
}

const std::map<std::string, float> Solver::getGoalsProba (int depth) const
{
    std::map<std::string, float> out = std::map<std::string, float>();
    for(std::map<int, std::vector<Output>>::const_iterator it = this->generators.begin(); it!= this->generators.end(); ++it)
    {
        for(std::vector<Output>::const_iterator itOutput = it->second.begin(); itOutput != it->second.end(); ++itOutput)
        {
            if(out.count(this->domain.getPlanLibrary().getLiteralName(itOutput->getGoals()[0])) == 0.0)
                out[this->domain.getPlanLibrary().getLiteralName(itOutput->getGoals()[0])] = 0.0;

            out[this->domain.getPlanLibrary().getLiteralName(itOutput->getGoals()[0])]++;
        }

    }
    for(std::map<std::string, float>::iterator it = out.begin(); it != out.end(); ++it)
    {
        it->second = it->second*100.0/(float)this->siz;
    }
    return out;
}

Solver& Solver::operator=(const Solver& other) // copy assignment
{
    if (this != &other) { // self-assignment check expected
        this->domain = other.domain;
        this->generators.insert(other.generators.begin(),other.generators.end());
        this->depth = other.depth;
        this->siz = other.siz;
        this->currentObservation = std::pair<int,int>(other.currentObservation.first,other.currentObservation.second);
        std::map<int,std::vector<std::pair<float,float>>> timeMap;
        this->timeMap.insert(other.timeMap.begin(),other.timeMap.end());
    }
    return *this;
}
