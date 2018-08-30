#include "../include/Domain.h"
#include <iostream>

Domain::Domain(float obsRate, float mutRate, float addRate, PlanLibrary planLibrary, std::unordered_map<int,std::unordered_map<int,float>> decisionModel )
{
    this->planLibrary = planLibrary;
    this->decisionModel = decisionModel;
    this->obsRate = obsRate;
    this->mutRate = mutRate;
    this->addRate = addRate;
    this->priorProbability = std::unordered_map<int,float>();
}

PlanLibrary Domain::getPlanLibrary() const
{
    return this->planLibrary;
}

ProductionRule Domain::getRandomProduction(int literal)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    float rndValue = std::uniform_real_distribution<>(0.0, 1.0)(gen);
    float probValue = 0.0;
    for(std::unordered_map<int,float>::iterator it = this->decisionModel[literal].begin(); it != this->decisionModel[literal].end();++it)
    {
        probValue += it->second;
        if(rndValue < probValue)
            return this->planLibrary.getProductionRule(it->first);
    }
    return ProductionRule();
}

int Domain::addRule(ProductionRule productionRule, std::unordered_map<int, ActionTime> times)
{
    return this->planLibrary.addRule(productionRule, times);
}

void Domain::addLiteral(std::string name, bool isTerminal, bool isGoal)
{
    this->planLibrary.addLiteral(name, isTerminal, isGoal);
}

int Domain::addRule(std::string primitive, std::vector<std::string> children, std::vector<std::pair<std::string,std::string>> constraints, float prob, std::unordered_map<int, ActionTime> times)
{
    int id = this->planLibrary.getAllRules().size();
    int intPrimitive = this->planLibrary.getLiteralId(primitive);
    std::unordered_set<int> setChildren =  std::unordered_set<int>();
    std::unordered_map<int,std::vector<int>> mapConstraints = std::unordered_map<int,std::vector<int>>();

    for(std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it)
    {
        setChildren.insert(this->planLibrary.getLiteralId(*it));
        mapConstraints[this->planLibrary.getLiteralId(*it)] = std::vector<int>();
    }

    for(std::vector<std::pair<std::string,std::string>>::iterator it = constraints.begin(); it != constraints.end(); ++it)
    {
        mapConstraints[this->planLibrary.getLiteralId(it->second)].push_back(this->planLibrary.getLiteralId(it->first));
    }

    ProductionRule productionRule = ProductionRule(id, intPrimitive,setChildren,mapConstraints);

    if (this->decisionModel.count(intPrimitive) == 0)
        this->decisionModel[intPrimitive] = std::unordered_map<int,float> ();
    this->decisionModel[intPrimitive][id] = prob;

    return this->addRule(productionRule, times);
}

void Domain::setPriorProbability(std::vector<std::pair<std::string, float>> priorProb)
{
    for(std::vector<std::pair<std::string, float>>::iterator it = priorProb.begin(); it!=priorProb.end(); ++it)
    {
        this->priorProbability[this->getPlanLibrary().getLiteralId(it->first)] = it->second;
    }
}

int Domain::getRandomGoal()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    float rndValue = std::uniform_real_distribution<>(0.0, 1.0)(gen);
    float probValue = 0.0;
    for(std::unordered_map<int,float>::iterator it = this->priorProbability.begin(); it != this->priorProbability.end();++it)
    {
        probValue += it->second;
        if(rndValue < probValue)
            return it->first;
    }
}

void Domain::setProbProduction(int productionRule, float proba)
{
    int primitive = this->getPlanLibrary().getProductionRule(productionRule).getPrimitive();
   // std::cout << primitive<<std::endl;
    if (this->decisionModel.count(primitive) == 0)
        this->decisionModel[primitive] = std::unordered_map<int,float> ();
    this->decisionModel[primitive][productionRule] = proba;
    //std::cout << this->decisionModel[primitive][productionRule]<<std::endl;
}
