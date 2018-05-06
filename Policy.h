//
// Created by uqamportable on 29/04/18.
//

#ifndef IMAGEREC_POLICY_H
#define IMAGEREC_POLICY_H

#include <Python.h>
#include <string>
#include "Affordance.h"
#include <sstream>


class Policy {

public:
    Policy();

    bool load(std::string policy) const;
    bool update(Affordance observation) const;
    Affordance getNextAction() const;
private:
    PyObject* main;
    PyObject* solver;


};


#endif //IMAGEREC_POLICY_H
