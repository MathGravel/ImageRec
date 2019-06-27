#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include "planLibrary.h"

using namespace std;

#ifndef PROBABILITYDISTRIBUTION_H
#define PROBABILITYDISTRIBUTION_H


class probabilityDistribution
{
    public:
        probabilityDistribution();
        probabilityDistribution(unordered_map<int, unordered_map<int,float>> _distribution);
        probabilityDistribution(planLibrary& _pl);
        probabilityDistribution(unordered_set<int> _terminals, float noise =0.0);
        virtual ~probabilityDistribution();

        bool isValid() const;
        int RNC(int i) const;

        void setProb(int primitive, int element, float probability);
        void setNoise(int Prct);

        const unordered_map<int, unordered_map<int,float>>& getDistribution() const {return distribution;};

    private:
        unordered_map<int, unordered_map<int,float>> distribution;

};
#endif // PROBABILITYDISTRIBUTION_H
