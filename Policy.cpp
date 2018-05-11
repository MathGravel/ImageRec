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
    //PyRun_SimpleString("mainSolver.loadPolicy(\"Beverage.dot\")");




}

bool Policy::update(Affordance observation) const {

    std::stringstream ss;
    ss << "mainSolver.add(\"" << observation.getName() << "\"," << observation.getObjectProbability() << ")";
    std::cout << ss.str();
    PyRun_SimpleString(ss.str().c_str());

    return true;
}

bool Policy::load(std::string policyLocation) const {

    std::stringstream ss;
    ss << "mainSolver.loadPolicy(\"" << policyLocation << " \")";
    PyRun_SimpleString(ss.str().c_str());
    return true;
}

std::string Policy::getCurrentPlan() const {


    PyRun_SimpleString("ret = mainSolver.policy.currentPlan");
    PyRun_SimpleString("pr = mainSolver.policy.currentPlanProb");

    std::stringstream ss;


    PyObject* plan = PyObject_GetAttrString(main,"ret");
    PyObject* prob = PyObject_GetAttrString(main,"pr");

    PyObject * temp_bytes = PyUnicode_AsEncodedString(plan, "UTF-8", "strict"); // Owned reference
    //PyObject * temp_bytesprob = PyUnicode_AsEncodedString(prob, "UTF-8", "strict"); // Owned
    double a = PyFloat_AsDouble(prob);


    if (temp_bytes != NULL) {
        char * my_result = PyBytes_AsString(temp_bytes); // Borrowed pointer
        my_result = strdup(my_result);
        ss << "Current Plan : " << my_result << "\t";

        //char * my_result1 = PyBytes_AsString(temp_bytesprob);
        //my_result1 = strdup(my_result1);
        ss << "Prob : " << a;
        Py_DecRef(temp_bytes);
        //Py_DecRef(temp_bytesprob);
        Py_DecRef(plan);
        Py_DecRef(prob);

    } else {
        ss << "Ca marche pas";
    }

    return ss.str();
}

std::string Policy::getNextAction() const {


    PyRun_SimpleString("act = mainSolver.policy.nextPossibleAction()");


    std::stringstream ss;

    ss << "Next expected action(s) : ";

    PyObject* plan = PyObject_GetAttrString(main,"act");
    PyObject * temp_bytes = PyUnicode_AsEncodedString(plan, "UTF-8", "strict"); // Owned reference

    if (temp_bytes != NULL) {
        char * my_result = PyBytes_AsString(temp_bytes); // Borrowed pointer
        my_result = strdup(my_result);
        Py_DecRef(temp_bytes);
        Py_DecRef(plan);

        ss << my_result;

    } else {
        ss << "Ca marche pas";
    }

    return ss.str();
}