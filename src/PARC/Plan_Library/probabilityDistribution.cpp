#include "probabilityDistribution.h"
#include <random>

probabilityDistribution::probabilityDistribution()
{
}

probabilityDistribution::probabilityDistribution(unordered_map<int, unordered_map<int,float>> _distribution)
{
    distribution = _distribution;
}

probabilityDistribution::probabilityDistribution(planLibrary& _pl)
{
    for(auto it : _pl.getRules())
    {
        if(distribution.count(it.second.getPrimitive())==0)
        {
            distribution[it.second.getPrimitive()][it.first] = 1.0;
        }
        else
        {
            int siz = distribution[it.second.getPrimitive()].size();
            for(auto& dit : distribution[it.second.getPrimitive()])
            {
                dit.second = (float) 1.0/(siz + 1);
            }
           distribution[it.second.getPrimitive()][it.first] = 1.0/(siz + 1);
        }
    }
}

probabilityDistribution::probabilityDistribution(unordered_set<int> _terminals, float noise )
{
    for(auto t_i: _terminals)
    {
        if(noise == 0.0)
        {
            distribution[t_i][t_i] =1.0;
        }
        else
        {
            for(auto t_j: _terminals)
            {
                if(t_i == t_j)
                    distribution[t_i][t_j] =1.0-noise;
                else
                    distribution[t_i][t_j] = noise/(_terminals.size()-1);
            }
        }
    }

}

probabilityDistribution::~probabilityDistribution()
{
    //dtor
}

bool probabilityDistribution::isValid() const
{
    for(auto it : distribution)
    {
        float sum = 0.0;
        for(auto dit : it.second)
        {
            sum += dit.second;
        }
        if(sum != 1.0)
            return false;
    }
    return true;
}


int probabilityDistribution::RNC(int i) const
{
    random_device rd;
    mt19937 e(rd());
    uniform_real_distribution<> dist(0,1);
    float sum = 0.0;
    float rand = dist(e);
    for(auto& it : distribution.at(i))
    {
        if(rand <= (sum+=it.second))
            return it.first;
    }
}

void probabilityDistribution::setProb(int primitive, int element, float probability)
{
    distribution[primitive][element] = probability;
}

