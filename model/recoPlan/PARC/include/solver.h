/**
 * \file      planLibrary.h
 * \author    Jean Massardi
 * \version   2.0
 * \date      8 August 2019
 * \brief     
 * \details   
*/
#include "extendedPlanLibrary.h"
#include "rule.h"
#include <vector>
#include <map>
#include <iostream>

#ifndef SOLVER_H
#define SOLVER_H

using namespace std;

class treeNode
{
    public:
    treeNode();
    treeNode(extendedPlanLibrary* _epl, int _symbol);

    extendedPlanLibrary* epl;
    int symbol;
    int _rule;
    bool status;
    map<int,treeNode> children;

    int update();

};

class tree
{
    public:
        tree();
        tree(extendedPlanLibrary* _epl, int goal);

        extendedPlanLibrary* epl;
        treeNode root;
        vector<int> update();
        int updateFO();

};

class solverParticle
{
    public:
        solverParticle(extendedPlanLibrary* _epl);

        extendedPlanLibrary* epl;
        int goal;
        vector<int> expNextObs;
        tree planTree;
        bool update();
};

class solver
{
    public:
        solver();
        solver(extendedPlanLibrary* _epl, int _nbParticle);
        virtual ~solver();

        bool addObservation(int obs);
        bool addObservation(std::string obs);
        bool status();
        map<int, int> getGoals();
        map<int, int> getParticles();
        map<std::string,float> getProbGoals();
        map<std::string,float> getProbParticles();
        int getMaxGoal();
        const int& getSize() const;
        void clear();

        pair<int,vector<int>> generatePlan();
        pair<int,vector<int>> generatePlanFO();

    private:
        int nbParticles;
        extendedPlanLibrary* epl;
        vector<int> previousObservations;
        map<int, vector<solverParticle>> particles;

};

#endif // SOLVER_H
