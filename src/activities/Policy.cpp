#include "Policy.h"

Policy::Policy() {

    /*Py_Initialize();
    PyRun_SimpleString("import sys");
    std::cout <<  Py_GetVersion() << std::endl;
    PyRun_SimpleString("import numpy");
    PyRun_SimpleString("import codecs");
    PyRun_SimpleString("import pydot");
    PyRun_SimpleString("import socket");
    PyRun_SimpleString("import os");

    std::ostringstream python_path;
    python_path<<"sys.path.append(\""<<"/home/uqamportable/Documents/ImageRec/PlanRecoByPolicy/\")";
    main = PyImport_AddModule("__main__");
    PyRun_SimpleString(python_path.str().c_str());
    PyRun_SimpleString("from GraphNavigator import Policy");
    PyRun_SimpleString("from PolicyGenerator import Domain");
    PyRun_SimpleString("from subprocess import call");
    PyRun_SimpleString("from Solver import *");
    PyRun_SimpleString("mainSolver = solver()");
    solver = PyObject_GetAttrString(main,"mainSolver");

    PyRun_SimpleString("mainSolver.loadPolicy(\"planBib.dot\")");
    //PyRun_SimpleString("mainSolver.loadPolicy(\"Beverage.dot\")");

    textFile = std::ofstream("trace/trace.txt");*/



}

Policy::~Policy() {
    textFile.close();
}

bool Policy::update(Affordance observation)  {

    /*std::stringstream ss;
    ss << "mainSolver.add(\"" << observation.getName() << "\"," << observation.getObjectProbability() << ")";
    std::cout << ss.str();
    PyRun_SimpleString(ss.str().c_str());
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    std::stringstream sss;

    sss  << now->tm_hour << ' ' << now->tm_min << ' ' << now->tm_sec  <<": ";
    sss  << observation.getName() << " " << observation.getObjectProbability() << std::endl;
    textFile << sss.str();
    return true;*/
}

bool Policy::load(std::string policyLocation) const {

    /*std::stringstream ss;
    ss << "mainSolver.loadPolicy(\"" << policyLocation << " \")";
    PyRun_SimpleString(ss.str().c_str());
    return true;
    */
}

std::string Policy::getCurrentPlan() const {

    /*
    PyRun_SimpleString("ret = mainSolver.policy.currentPlan");

    std::stringstream ss;


    PyObject* plan = PyObject_GetAttrString(main,"ret");

    PyObject * temp_bytes = PyUnicode_AsEncodedString(plan, "UTF-8", "strict"); // Owned reference



    if (temp_bytes != NULL) {
        char * my_result = PyBytes_AsString(temp_bytes); // Borrowed pointer
        my_result = strdup(my_result);
        ss  << my_result;
        Py_DecRef(temp_bytes);
        Py_DecRef(plan);

    } else {
        ss << "Ca marche pas";
    }

    return ss.str();
       */
}

std::string Policy::getCurrentPlanProb() const {

    /*
    PyRun_SimpleString("pr = mainSolver.policy.currentPlanProb");

    std::stringstream ss;


    PyObject* prob = PyObject_GetAttrString(main,"pr");

    double a = PyFloat_AsDouble(prob);


    if (a != 0) {
        ss << "Prob : " << a;
        Py_DecRef(prob);

    } else {
        ss << "Ca marche pas";
    }

    return ss.str();
       */
}



std::string Policy::getNextAction() const {

    /*
    PyRun_SimpleString("act = mainSolver.policy.nextPossibleAction()");

    std::stringstream ss;

    PyObject* plan = PyObject_GetAttrString(main,"act");
    PyObject * temp_bytes = PyUnicode_AsEncodedString(plan, "UTF-8", "strict"); // Owned reference

    if (temp_bytes != NULL) {
        char * my_result = PyBytes_AsString(temp_bytes); // Borrowed pointer
        my_result = strdup(my_result);
        Py_DecRef(temp_bytes);
        Py_DecRef(plan);

        ss << my_result;
        std::string temp = ss.str();
        temp =getFirst(temp);
        ss.str("");
        ss << temp;


    } else {
        ss << "Ca marche pas";
    }

    return ss.str();
       */
}
