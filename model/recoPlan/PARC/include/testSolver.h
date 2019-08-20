/**
 * \file      planLibrary.h
 * \author    Jean Massardi
 * \version   2.0
 * \date      8 August 2019
 * \brief     
 * \details   
*/
#include <vector>
#include <map>
#include <utility>
#include "solver.h"
#include "extendedPlanLibrary.h"

#ifndef TESTSOLVER_H
#define TESTSOLVER_H

using namespace std;

class testSolver
{
    public:
        testSolver(int _nbTest);
        virtual ~testSolver();
        void run();
        void print();
        void addSolver(solver s);

    private:
        int nbTests;
        extendedPlanLibrary current_epl;
        vector<solver> solvers;
        extendedPlanLibrary epl;
        vector<vector<int>> results;
        vector<int> timeSecond;

};

#endif // TESTSOLVER_H
