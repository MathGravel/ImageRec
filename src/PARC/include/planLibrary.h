#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include "rule.h"

using namespace std;

#ifndef PLANLABRARY_H
#define PLANLABRARY_H


class planLibrary
{
    public:
        planLibrary();
        planLibrary(const planLibrary& _pl);
        planLibrary(const char* spl);
        virtual ~planLibrary();

        bool isTerminal(int symbol) const;
        bool isGoal(int symbol) const;

        int addSymbol(int s, bool t, bool g);
        int addRule(rule r);

        const string toString();

        const unordered_set<int>& getTerminals() const;
        const unordered_set<int>& getNonTerminals() const;
        const unordered_set<int>& getGoals() const;
        const unordered_map<int,rule>& getRules() const;

    private:
        unordered_set<int> terminals;
        unordered_set<int> nonTerminals;
        unordered_set<int> goals;
        unordered_map<int,rule> rules;
};

#endif // PLANLABRARY_H
