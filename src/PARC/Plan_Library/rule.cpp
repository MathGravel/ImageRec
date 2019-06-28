#include <vector>
#include <utility>
#include "rule.h"
#include <string>

rule::rule():id(-1),primitive(-1)
{
}

rule::rule(int _primitive, int _id):id(_id), primitive(_primitive)
{
}

rule::rule(int _id, string s):id(_id)
{
    size_t pos = 0;
    if((pos = s.find(":")) != string::npos)
    {
        primitive = stoi(s.substr(0,pos));
    }
    s.erase(0, pos + 1);
    pos = s.find("|");
    string s_children = s.substr(0, pos);
    string s_constraints = s.substr(pos+1, s.find(";"));

    while ((pos = s_children.find(",")) != std::string::npos)
    {
        children.push_back(stoi(s.substr(0, pos)));
        s_children.erase(0, pos + 1);
    }

    while ((pos = s_constraints.find("),")) != std::string::npos)
    {
        int _first;
        int _second;
        string temp;
        size_t vir;

        temp = s_constraints.substr(1,pos);
        vir = temp.find(",");

        _first = stoi(temp.substr(0,vir));
        _second = stoi(temp.erase(0,vir+1));

        constraints.push_back(make_pair(_first,_second));
        s_children.erase(0, pos + 2);
    }


}

void rule::addChild(int child)
{
    children.push_back(child);
}

void rule::addChildren(vector<int> _children)
{
    children.insert(children.end(), _children.begin(), _children.end());
}

void rule::addConstraint(pair<int, int> constraint)
{
    constraints.push_back(constraint);
}

void rule::addConstraints(vector<pair<int,int>> _constraints)
{
    constraints.insert(constraints.end(), _constraints.begin(), _constraints.end());
}

rule::operator int() const
{
    return id;
}

bool rule::operator<(const rule _r) const
{
    return id<(int)_r;
}

bool rule::operator==(const rule _r) const
{
    return id==(int)_r;
}

const int rule::getPrimitive() const
{
    return primitive;
}

const vector<int>& rule::getChildren() const
{
    return children;
}

const vector<pair<int,int>>& rule::getConstraints() const
{
    return constraints;
}

const string rule::toString()
{
    string resu = "";
    resu+=std::to_string(this->primitive);
    resu+="(";
    for(int i = 0; i<this->children.size();i++)
    {
        resu += std::to_string(this->children[i]);
        if(i != this->children.size()-1)
            resu += ",";
    }
    resu+="),(";
    for(int j = 0; j<this->constraints.size();j++)
    {
        resu+= "(";
        resu+= std::to_string(this->constraints[j].first);
        resu+= ",";
        resu+= std::to_string(this->constraints[j].second);
        resu+= ")";
        if(j != this->constraints.size()-1)
            resu += ",";
    }
    resu+=")";
    return resu;
}

const string rule::toString(map<int,string> revIds)
{
    string resu = "";
    resu+=revIds[this->primitive];
    resu+="(";
    for(int i = 0; i<this->children.size();i++)
    {
        resu += revIds[this->children[i]];
        if(i != this->children.size()-1)
            resu += ",";
    }
    resu+="),(";
    for(int j = 0; j<this->constraints.size();j++)
    {
        resu+= "(";
        resu+= std::to_string(this->constraints[j].first);
        resu+= ",";
        resu+= std::to_string(this->constraints[j].second);
        resu+= ")";
        if(j != this->constraints.size()-1)
            resu += ",";
    }
    resu+=")";
    return resu;
}

std::ostream& operator<<(std::ostream& os, const rule& r)
{
    os << r.primitive << ":";
    for(int i = 0; i<r.children.size();i++)
    {
        os << r.children[i];
        if(i != r.children.size()-1)
            os << ",";
    }
    os << "|";
    for(int j = 0; j<r.constraints.size();j++)
    {
        os << "(" << r.constraints[j].first << ","<< r.constraints[j].second << ")";
        if(j != r.constraints.size()-1)
            os << ",";
    }
    os << ";";
}

rule::~rule()
{
    //dtor
}
