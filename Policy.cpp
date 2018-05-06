//
// Created by uqamportable on 29/04/18.
//

#include "Policy.h"

Policy::Policy() {

    Py_Initialize();
    PyRun_SimpleString("import sys");
    std::cout <<  Py_GetVersion() << std::endl;
    PyRun_SimpleString("import numpy");
    PyRun_SimpleString("import codecs");
    PyRun_SimpleString("import pydot");
    PyRun_SimpleString("import socket");
    PyRun_SimpleString("import os");

    std::ostringstream python_path;
    python_path<<"sys.path.append(\""<<"/home/uqamportable/CLionProjects/ImageRec/PlanRecoByPolicy/\")";
    main = PyImport_AddModule("__main__");
    PyRun_SimpleString(python_path.str().c_str());
    PyRun_SimpleString("from GraphNavigator import Policy");
    PyRun_SimpleString("from PolicyGenerator import Domain");
    PyRun_SimpleString("from subprocess import call");
    PyRun_SimpleString("from Solver import *");
    PyRun_SimpleString("mainSolver = solver()");
    solver = PyObject_GetAttrString(main,"mainSolver");

    PyRun_SimpleString("mainSolver.loadPolicy(\"Beverage.dot\")");
    PyRun_SimpleString("mainSolver.loadPolicy(\"Beverage.dot\")");




}

bool Policy::update(Affordance observation) const {

    std::stringstream ss;
    ss << "mainSolver.add(\"" << observation << " \")";
    PyRun_SimpleString(ss.str().c_str());

    return true;
}

bool Policy::load(std::string policyLocation) const {

    std::stringstream ss;
    ss << "mainSolver.loadPolicy(\"" << policyLocation << " \")";
    PyRun_SimpleString(ss.str().c_str());
    return true;
}

Affordance Policy::getNextAction() const {


    PyRun_SimpleString("ret = mainSolver.policy.currentPlan");



    PyObject* plan = PyObject_GetAttrString(main,"ret");
    PyObject * temp_bytes = PyUnicode_AsEncodedString(plan, "UTF-8", "strict"); // Owned reference


    if (temp_bytes != NULL) {
        char * my_result = PyBytes_AsString(temp_bytes); // Borrowed pointer
        my_result = strdup(my_result);
        Py_DecRef(temp_bytes);
        Py_DecRef(plan);
        std::cout << my_result << std::endl;

    } else {
        std::cout << "Ca marche pas";
    }

    std::cout << "ret";
    return Affordance();
}