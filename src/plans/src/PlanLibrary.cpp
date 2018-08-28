#include "../include/Domain.h"
#include <stdexcept>
#include <iostream>
#include <assert.h>

PlanLibrary::PlanLibrary()
{
    this->literalMap = std::map<int,std::string>();
    this->terminalActions =  std::unordered_set<int>();
    this->nonTerminalActions =   std::unordered_set<int>();
    this->goals =   std::unordered_set<int>();
    this->productionRules =  std::unordered_map<int,ProductionRule>();
    this->ActionTimes = std::unordered_map<int,std::unordered_map<int, ActionTime>>();
}

ProductionRule PlanLibrary::getProductionRule(int id)
{
    return this->productionRules[id];
}

 std::unordered_map<int,ProductionRule> PlanLibrary::getAllRules()
{
    return this->productionRules;
}

void PlanLibrary::addLiteral(std::string name, bool isTerminal, bool isGoal)
{
    if(this->getLiteralId(name) != -1)
        //throw std::invalid_argument( "Already in the Plan library" );
        return;
    this->literalMap[this->literalMap.size()] = name;
    if(isTerminal)
        this->terminalActions.insert(this->literalMap.size()-1);
    else
        this->nonTerminalActions.insert(this->literalMap.size()-1);

    if(isGoal)
        this->goals.insert(this->literalMap.size());

}

int PlanLibrary::addRule(ProductionRule productionRule, std::unordered_map<int, ActionTime> times = std::unordered_map<int, ActionTime>())
{
    this->productionRules[int(productionRule)] = productionRule;
    if(!times.empty())
        this->ActionTimes[int(productionRule)] = times;
    return int(productionRule);
}

bool PlanLibrary::isTerminal(std::string literal)
{
    return this->isTerminal(this->getLiteralId(literal));
}

bool PlanLibrary::isTerminal(int literal)
{
    return (this->terminalActions.count(literal) > 0) ;
}

std::string PlanLibrary::getLiteralName(int literal)
{
    return this->literalMap[literal];
}

int PlanLibrary::getLiteralId(std::string literal)
{
    for(std::map<int,std::string>::iterator it = this->literalMap.begin(); it != this->literalMap.end(); ++it)
    {
        if(literal == it->second)
            return it->first;
    }
    return -1;
}

int PlanLibrary::getRandomTerminal()
{
    int randomIndex = rand() % this->terminalActions.size();
    int index = 0;
    for(std::unordered_set<int>::iterator it = this->terminalActions.begin(); it!= this->terminalActions.end(); ++it)
    {
        if(index == randomIndex)
            return *it;
        index++;
    }
}

std::string PlanLibrary::to_string()
{
    std::string out = "PLAN LIBRARY" + '\n';

    out += "Literal map" + '\n';
    for(std::map<int,std::string>::iterator it = this->literalMap.begin(); it!= this->literalMap.end(); ++it)
    {
        out += std::to_string(it->first) + " " + it->second + " " +std::to_string(this->isTerminal(it->first))+'\n' ;
    }

    out += "Terminal" + '\n';

    out += "Non terminal" + '\n';

    out += "Goals" + '\n';

    return out;
}
