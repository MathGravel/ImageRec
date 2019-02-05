#ifndef SOLVER_H
#define SOLVER_H

#include <Domain.h>
#include <Generator.h>

class Solver
{
	private:
        Domain domain;
        std::map<int, std::vector<Output>> generators;
        int depth;
        int siz;
        std::pair<int, int> currentObservation;
        std::map<int,std::vector<std::pair<float,float>>> timeMap;
	public:
        Solver();
        Solver(Domain domain, int depth = 1, int siz = 10000);
        bool addObservation (std::string observation);
        bool addObservation (int lit);
        const std::map<int, std::vector<Output>> getGenerators() const {return generators;};
        const std::map<std::string, float> getNextActions(int depth = 1)const;
        const std::map<std::string, float> getGoalsProba (int depth = 1) const;
        const std::map<int, std::vector<std::pair<float,float>>> getTimeMap() const {return timeMap;};
        Solver& operator=(const Solver& other);
        std::string TerminalGoal;

};

#endif // SOLVER_H
