#ifndef DOMAIN_H
#define DOMAIN_H

#include <random>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <string>
#include <utility>

enum Prob_law:char
{
    u /*uniform*/,
    n /*normal*/,
    e /*exponantial*/
};

class ActionTime
{
    public:
        ActionTime(Prob_law prob_law, float param1, float param2):prob_law(prob_law), param1(param1), param2(param2){};
        ActionTime():prob_law(u), param1(0.0),param2(0.0){};
        ActionTime(std::string str);
        const std::string to_string() const;
        bool operator == ( const ActionTime& actt) const;
        float rnd_value();
    private:
        Prob_law prob_law;
        float param1;
        float param2;

};

class ProductionRule
{
    public:
        ProductionRule(int id, int primitive, std::unordered_set<int> children, std::unordered_map<int,std::vector<int>> constraints);
        ProductionRule();
        int getPrimitive();
        std::unordered_set<int> getChildren() const;
        std::unordered_map<int,std::vector<int>>getConstraints();
        bool operator<(ProductionRule const& productionRule) const;
        operator int () const;
        std::string to_string();
    private:
        int id;
        int primitive;
        std::unordered_set<int> children;
        std::unordered_map<int,std::vector<int>> constraints; //WARNING : the key represent the second element, all the constraints are reversed
};

class PlanLibrary
{
    public:
        PlanLibrary();
        void addLiteral(std::string name, bool isTerminal, bool isGoal);
        int addRule(ProductionRule productionRule, std::unordered_map<int, ActionTime> times);
        bool isTerminal(std::string literal);
        bool isTerminal(int literal);
        std::string getLiteralName(int literal);
        int getLiteralId(std::string literal);
        ProductionRule getProductionRule(int id);
        std::unordered_map<int,ProductionRule> getAllRules();
        int getRandomTerminal();
        std::string to_string();
    private:
        std::map<int,std::string> literalMap;
        std::unordered_set<int> terminalActions;
        std::unordered_set<int> nonTerminalActions;
        std::unordered_set<int> goals;
        std::unordered_map<int,ProductionRule> productionRules;
        std::unordered_map<int,std::unordered_map<int, ActionTime>> ActionTimes;
};

class Domain
{
    public:
        Domain(float obsRate = 1.0, float mutRate = 0.0, float addRate = 0.0, PlanLibrary = PlanLibrary(), std::unordered_map<int,std::unordered_map<int,float>> decisionModel = std::unordered_map<int,std::unordered_map<int,float>> ());
        PlanLibrary getPlanLibrary() const;
        ProductionRule getRandomProduction(int literal);
        void addLiteral(std::string name, bool isTerminal, bool isGoal);
        int addRule(ProductionRule productionRule, std::unordered_map<int, ActionTime> times = std::unordered_map<int, ActionTime>());
        int addRule(std::string primitive, std::vector<std::string> children, std::vector<std::pair<std::string,std::string>> constraints, float prob, std::unordered_map<int, ActionTime> times = std::unordered_map<int, ActionTime>());
        void setPriorProbability(std::vector<std::pair<std::string, float>> priorProb);
        int getRandomGoal();
        void setProbProduction(int productionRule, float proba);
        float getObsRate() const {return obsRate;};
        float getAddRate() const {return addRate;};
        float getMutRate() const {return mutRate;};
    private:
        PlanLibrary planLibrary;
        float obsRate;
        float mutRate;
        float addRate;
        std::unordered_map<int,std::unordered_map<int,float>> decisionModel;
        std::unordered_map<int, float> priorProbability;
};

class SmallDomain
{
    public:
    static Domain getSmallDomain();
    int test();
    std::string getMax(std::map<std::string, float> probDistribution);
    Domain teaDomain();


};

#endif // DOMAIN_H
