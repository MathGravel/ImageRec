#ifndef POLICY_H
#define POLICY_H

//#include <Python.h>
#include <string>
#include "Affordance.h"
#include <sstream>
#include <iostream>
#include <fstream>


class Policy {

public:
    Policy();
    ~Policy();
    bool load(std::string policy) const;
    bool update(Affordance observation);
    std::string getNextAction() const;
    std::string getCurrentPlan() const;
    std::string getCurrentPlanProb() const;




private:
   // PyObject* main;
    //PyObject* solver;
    std::ofstream textFile;


    enum charTypeT{ other, alpha, digit};

    charTypeT charType(char c) const{
        if(isdigit(c))return digit;
        if(isalpha(c))return alpha;
        return other;
    }

    std::string separateThem(std::string inString) const{
        std::string oString = "";charTypeT st=other;
        for(auto c:inString){
            if( (st==alpha && charType(c)==digit) || (st==digit && charType(c)==alpha) )
                oString.push_back(' ');
            oString.push_back(c);st=charType(c);
        }
        return oString;
    }

    std::string getFirst(std::string inString) const{
        std::string oString = "";charTypeT st=other;
        int i = 0;
        for(auto c:inString){
            if( (st==alpha && charType(c)==digit) || (st==digit && charType(c)==alpha) ) {
                oString.push_back(' ');
                i++;
                if (i > 1)
                    break;
            }
            oString.push_back(c);st=charType(c);
        }
        return oString;
    }


};


#endif //POLICY_H
