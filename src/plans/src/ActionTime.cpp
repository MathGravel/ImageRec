#include "../include/Domain.h"
#include "../include/Generator.h"
#include <random>


ActionTime::ActionTime(std::string str)
 {
        std::string temp = str;
        switch(temp[0])
                {
                    case ('u'):
                        this->prob_law = u;
                    case ('n'):
                        this->prob_law = n;
                    case ('e'):
                        this->prob_law = e;
                }
        temp.erase(0,1);
        this->param1 = stof(temp.substr(0,temp.find(",")));
        temp.erase(0, temp.find(",")+1);
        this->param2 = stof(temp.substr(0,temp.find(")")));
}

const std::string ActionTime::to_string() const
{
    std::string resu;
    switch(this->prob_law)
        {
            case (u):
                resu="u(";
            case (n):
                resu="n(";
            case (e):
                resu="e(";
        }
    resu.append(std::to_string(param1));
    resu.append(", ");
    resu.append(std::to_string(param2));
    resu.append(")");
    return resu;

}

bool ActionTime::operator==( const ActionTime& actt) const
{
    return (this->to_string() == actt.to_string());
}

float ActionTime::rnd_value()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    switch(this->prob_law)
    {
        case (u):
            return (std::uniform_real_distribution<>(this->param1, this->param2)(gen));
        case (n):
            return (std::normal_distribution<>(this->param1, this->param2)(gen));
        case (e):
            return (param1 + std::exponential_distribution<>(param2)(gen));
    }
}

