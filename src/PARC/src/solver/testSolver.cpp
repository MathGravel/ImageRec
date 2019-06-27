#include "testSolver.h"

testSolver::testSolver(int _nbTest):nbTests(_nbTest)
{
    //current_epl = extendedPlanLibrary(true);
    for(int nbPart = 50; nbPart <= 500; nbPart+=50)
    {
        solvers.push_back(solver(&current_epl,nbPart));
        results.push_back(vector<int>());
        for(int i = 0; i < nbTests; i++)
        {
            results[(nbPart/50)-1].push_back(-1);
        }
    }
}

void testSolver::run()
{
    for(int i =0; i < nbTests;i++)
    {
        //current_epl = extendedPlanLibrary();
        //cout << "tS epl OK" <<endl;
        pair<int,vector<int>> current_plan = solvers[0].generatePlan();
        //cout << "tS CURRENT_PLAN OK " <<current_plan.first << " : " <<endl;
        /*for(int obs = 0; obs < current_plan.second.size(); obs++)
        {
            cout << current_plan.second[obs] << " ";
        }*/
        cout << endl;
        for(int j = 0; j < solvers.size(); j++)
        {
            solvers[i].clear();
            results[j][i] = -1;
        }
        for(int obs = 0; obs < current_plan.second.size(); obs++)
        {

            for(int j = 0; j < solvers.size(); j++)
            {
                if(results[j][i] == -1)
                {
                    if(solvers[j].getMaxGoal() == current_plan.first)
                        results[j][i] = obs;
                    else if(!solvers[j].addObservation(current_plan.second[obs]))
                    {
                        results[j][i] = -2;
                    }
                }
            }
        }
        cout << "**test : " << i << "**"<<endl;
        cout << "50 100 150 200 250 300 350 400 450 500" <<endl;
        for(int j = 0; j < solvers.size(); j++)
        {
            cout << results[j][i] << " ";
        }
        cout << endl;

    }



}

void testSolver::print()
{

}

void testSolver::addSolver(solver s)
{
    solvers.push_back(s);
}

testSolver::~testSolver()
{
    //dtor
}
