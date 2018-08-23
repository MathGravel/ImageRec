#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <random>
#include <iostream>
#include "Domain.h"

class OutputNode
{
    public:
        OutputNode(int literal, Domain* domain);
        OutputNode(OutputNode const &OutputNode);
        OutputNode();
        int expandOutput();
        bool isFinished() const {return finish;};
        ProductionRule getProduction() const {return productionRule;};
        int getLiteral() const {return literal;};
        std::vector<OutputNode> getChildren() const {return listChildren;};
    private:
        Domain* domain;
        std::vector<OutputNode> listChildren;
        int literal;
        ProductionRule productionRule;
        OutputNode* selectValidSymbol();
        bool finish;
        bool isValid (int lit) const;
};

class Output
{
    public:
        Output(Domain* domain);
        //Output(Output const &Output);
        bool expand();
        const std::vector<int>& getPOOutput()const {return this->POOutput;};
        const std::vector<int>& getFOOutput()const {return this->FOOutput;};
        std::pair<float, float> getTimeDurationAction(int i) {return this->timeDurationOutput[i];};
        int getLastActionFO(){return FOOutput[FOOutput.size()-1];};
        int getLastActionPO(){return POOutput[POOutput.size()-1];};
        const std::vector<int>& getGoals()const {return Goals;};
    private:
    	Domain* domain;
        std::vector<int> FOOutput;
        std::vector<std::pair<float, float>> timeDurationOutput;
        std::vector<int> POOutput;
        std::vector<int> Goals;
        OutputNode treeOutput;
};


#endif // GENERATOR_H
