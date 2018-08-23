#include "../include/Domain.h"
#include <iostream>
ProductionRule::ProductionRule(int id, int primitive, std::unordered_set<int> children, std::unordered_map<int,std::vector<int>> constraints)
{
    this->id = id;
    this->primitive = primitive;
    this->children = children;
    this->constraints = constraints;
}

ProductionRule::ProductionRule()
{
    this->id = -1;
    this->primitive = -1;
    this->children = std::unordered_set<int>();
    this->constraints = std::unordered_map<int,std::vector<int>>();
}

ProductionRule::operator int () const
{
    return this->id;
}

int ProductionRule::getPrimitive()
{
    return this->id;
}

std::unordered_set<int> ProductionRule::getChildren() const
{
    return this->children;
}

std::unordered_map<int,std::vector<int>> ProductionRule::getConstraints()
{
    return this->constraints;
}

bool ProductionRule::operator<(ProductionRule const& productionRule) const
{
    return this->id < int(productionRule);
}

std::string ProductionRule::to_string()
{
    std::string out;
    out = "Production Rule" + '\n';
    out += std::to_string(this->id) + " " + std::to_string(this->primitive);
    out += "List children" + '\n';
    for(std::unordered_set<int>::iterator it = this->children.begin(); it != this->children.end();++it)
    {
        out += std::to_string(*it) + " ";
    }
    out += '\n';
    for(std::unordered_map<int,std::vector<int>>::iterator it = this->constraints.begin(); it!=this->constraints.end();++it)
    {

        out += std::to_string(it->first) + " : ";
        for(std::vector<int>::iterator itVec = it->second.begin(); itVec!= it->second.end(); ++itVec)
        {
            out += std::to_string(*itVec) + " ";
        }
        out += '\n';
    }
    return out;
}
