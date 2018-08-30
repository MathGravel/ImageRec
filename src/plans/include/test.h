#ifndef TESTING_H
#define TESTING_H
#include <string>
#include <algorithm>
#include <utility>
#include <vector>
#include "Domain.h"
#include "Solver.h"
#include <map>
#include "Generator.h"

class Testing
{
    public:
        Testing(int solverSize, int numberOfDomain = 10 ,int numberOfTest = 100, std::vector<float> observability = {1.0,0.0,0.0});
        Domain testDomain(int goalSize = 10, int actionSize = 10, int depth =3, int widthProd =3, int widthOr =2, float minProb = 0.0, float maxProb = 1.0 ,float rateConstraints =0.3, std::vector<float> observability = {1.0,0.0,0.0});
        void newDomain(int goalSize = 10, int actionSize = 100, int depth =4, int widthProd =3, int widthOr =2, float minProb = 0.0, float maxProb = 1.0 ,float rateConstraints =0.3, std::vector<float> observability = {1.0,0.0,0.0});
        void run();
    private:
        std::vector<std::string> createChildren(char level, int actionSize, int widthProd);
        std::vector<std::pair<std::string,std::string>> createConstraints(std::vector<std::string> children, float rateConstraints);
        std::vector<float> ruleProb(int size, float minProb, float maxProb);
        Domain currentDomain;
        Output currentSequence;
        Solver currentSolver;
        int solverSize;
        int currentIteration;
        int numberOfTest;
        int numberOfDomain;
        std::vector<float> observability;
        std::map<int, int> correctnessGoal;
        std::map<int, int> correctnessAction;
        std::string getMax(std::map<std::string, float> probDistribution);
};

#endif // TESTING_H
