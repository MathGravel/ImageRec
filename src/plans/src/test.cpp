#include "test.h"
#include <iostream>


Testing::Testing(int solverSize, int numberOfDomain,int numberOfTest, std::vector<float> observability):currentDomain(testDomain(10,100,3,3,2,0.0,1.0,0.3,observability)),currentSequence(Output(&currentDomain))
{
        while(this->currentSequence.expand())
        {
        }
        this->solverSize = solverSize;
        this->currentIteration = 0;
        this->numberOfTest = numberOfTest;
        this->numberOfDomain = numberOfDomain;
        this->observability = observability;
        this->correctnessGoal = std::map<int, int>();
        this->correctnessAction = std::map<int, int>();
        this->currentSolver = Solver(currentDomain,1,this->solverSize);
}

void Testing::newDomain(int goalSize, int actionSize, int depth , int widthProd , int widthOr , float minProb , float maxProb ,float rateConstraints ,std::vector<float> observability)
{
    this->currentDomain = testDomain(goalSize, actionSize, depth , widthProd , widthOr , minProb , maxProb ,rateConstraints ,observability);
    this->currentSequence = Output(&this->currentDomain);
    this->currentSolver = Solver(this->currentDomain,1, this->solverSize);
    while(this->currentSequence.expand())
    {
    }
}

std::string Testing::getMax(std::map<std::string, float> probDistribution)
{
    float maxValue =0.0;
    std::string resu;
    for(std::map<std::string, float>::const_iterator it = probDistribution.begin(); it != probDistribution.end(); it++)
    {
        if(it->second > maxValue)
        {
            maxValue = it->second;
            resu = it->first;
        }
    }
    return resu;
}

void Testing::run()
{
    while(this->currentIteration < (this->numberOfTest)*(this->numberOfDomain))
    {
        std::cout<< "Current Iteration " + std::to_string(this->currentIteration)<<std::endl;
        std::cout<<"--Init correctness maps"<<std::endl;
        for(int i = 0; i< this->currentSequence.getPOOutput().size(); i++)
        {
            this->correctnessGoal[i] = 0;
            this->correctnessAction[i] = 0;
        }

        std::cout<<"--Previous prior check"<<std::endl;
        std::string currentGoal = this->currentDomain.getPlanLibrary().getLiteralName(this->currentSequence.getGoals()[0]);
        if(currentGoal == getMax(this->currentSolver.getGoalsProba()))
        {
            this->correctnessGoal[0] +=1;
        }
    // for each POOutput
    std::cout<<"--Sequence Loop"<<std::endl;
    for(int i = 0; i< this->currentSequence.getPOOutput().size(); i++)
    {
        std::string currentAction = this->currentDomain.getPlanLibrary().getLiteralName(this->currentSequence.getPOOutput()[i]);//this->currentDomain->getPlanLibrary().getLiteralName(this->currentSequence.getPOOutput()[i]);
        std::cout<<"-- --current Action"<<std::endl;
        if(currentAction == getMax(this->currentSolver.getNextActions()))
        {
            this->correctnessAction[i] +=1;
        }
        //add observation
        std::cout<<"-- --addObservation"<<std::endl;
        this->currentSolver.addObservation(this->currentSequence.getPOOutput()[i]);
        //if correct() add at the corresponding address
         std::cout<<"-- --currentGoal"<<std::endl;
        if(currentGoal == getMax(this->currentSolver.getGoalsProba()))
        {
            this->correctnessGoal[i] +=1;
        }
    }
    // if %mod number of Domain

    std::cout<<"--Domain check"<<std::endl;
    if(this->currentIteration % this->numberOfDomain == 0)
    {
        this->currentDomain = testDomain(10,100,3,3,2,0.0,1.0,0.3,observability);
    }
        //create a new domain
    // create a new solver

    std::cout<<"--new solver"<<std::endl;
    this->currentSolver = Solver(this->currentDomain,1,this->solverSize);
    }
}


std::vector<std::string> Testing::createChildren(char level, int actionSize, int widthProd)
{
    std::vector<std::string> children = std::vector<std::string>();
    while(children.size() < widthProd)
    {
        //std::cout<<children.size()<<std::endl;
        int rnd = rand()%actionSize;
        if(std::count(children.begin(),children.end(),level + std::to_string(rnd)) == 0)
        {
            children.push_back(level + std::to_string(rnd));
        }
    }
    return children;
}

std::vector<std::pair<std::string,std::string>> Testing::createConstraints(std::vector<std::string> children, float rateConstraints)
{
    std::vector<std::pair<std::string,std::string>> constraints = std::vector<std::pair<std::string,std::string>> ();
    std::random_device rd;
    std::mt19937 gen(rd());
    for(int i = 1; i < children.size();i++)
    {
        for(int j = 0; j < i ; j++)
        {
            if(std::uniform_real_distribution<>(0.0, 1.0)(gen)<rateConstraints)
            {
                constraints.push_back(std::make_pair(children[j],children[i]));
            }
        }
    }
    return constraints;
}

std::vector<float> Testing::ruleProb(int size, float minProb, float maxProb)
{
    std::vector<float> resu = std::vector<float>();
    std::random_device rd;
    std::mt19937 gen(rd());
    float sumProb = 0.0;
    for(int i =0; i<size-1;i++)
    {
        resu.push_back(std::uniform_real_distribution<>(minProb, maxProb)(gen));
        sumProb += minProb;
    }

    for(int i =0; i<size-1;i++)
    {
        resu[i] = resu[i]/sumProb;
    }


    //TODO investigate performances
   /* while(sumProb < 1.0)
    {
        float rnd = std::uniform_real_distribution<>(0.0, maxProb-minProb)(gen);
        if(sumProb + rnd > 1.0)
        {
            resu[rand()%resu.size()] += 1.0-sumProb;
            sumProb = 1.0;
        }
        else
        {
             resu[rand()%resu.size()] += rnd;
             sumProb +=rnd;
        }
    }*/

    return resu;
}

Domain Testing::testDomain(int goalSize, int actionSize, int depth , int widthProd , int widthOr , float minProb , float maxProb ,float rateConstraints ,std::vector<float> observability)
{
    Domain domain = Domain(observability[0],observability[1],observability[2]);
    //init literal
    for(int i = 0; i < actionSize; i++)
    {
        domain.addLiteral("A" + std::to_string(i), true, false);
    }

    for(char level = 'B'; level < 'A' + depth - 1; ++level)
    {
        for(int i =0; i<actionSize; i++)
        {
            domain.addLiteral(level + std::to_string(i), false, false);
            std::vector<float> ruleP = ruleProb(widthOr,minProb,maxProb);
            for(int rule = 0 ; rule<widthOr; rule++)
            {
                std::vector<std::string> children =  createChildren(level-1, actionSize, widthProd);
                std::vector<std::pair<std::string,std::string>> constraints = createConstraints(children, rateConstraints);
                domain.addRule(level + std::to_string(i),children,constraints,ruleP[rule]);
            }
        }
    }

    std::vector<std::pair<std::string,float>> priority = std::vector<std::pair<std::string,float>> ();
    for(int i = 0; i < goalSize; i++)
    {
        domain.addLiteral("goal" + std::to_string(i), false, true);
        std::vector<float> ruleP = ruleProb(widthOr,minProb,maxProb);
        priority.push_back(std::make_pair("goal" + std::to_string(i),1.0/(float)goalSize));

        for(int rule = 0 ; rule<widthOr; rule++)
        {
            std::vector<std::string> children =  createChildren('A' + depth - 2, actionSize, widthProd);
            std::vector<std::pair<std::string,std::string>> constraints = createConstraints(children, rateConstraints);
            domain.addRule("goal"+ std::to_string(i),children,constraints,ruleP[rule]);
        }
    }

    domain.setPriorProbability(priority);

    return domain;
}
