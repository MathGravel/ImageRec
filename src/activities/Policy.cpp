#include "Policy.h"

Policy::Policy() {


    domain = SmallDomain::getSmallDomain();

    s = Solver(domain,1,500);
    int i = 0;
}

bool Policy::update(Affordance observation)  {
    s.addObservation("hold(" + observation.getName() + ")");
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

bool comp(const pair<string, float>& a1, const pair<string, float>& a2)
{
    return a1.second < a2.second;
}

std::vector<std::pair<std::string,float>> Policy::getNextActions()
{
    std::map<std::string, float> nextActions = s.getNextActions();
    std::vector<std::pair<std::string,float>> vec;
    std::copy(nextActions.begin(), nextActions.end(), std::back_inserter<std::vector<std::pair<std::string,float>>>(vec));
    std::sort(vec.begin(), vec.end(), [](const std::pair<std::string,float>& l, const std::pair<std::string,float>& r) {
        if (l.second != r.second)
            return l.second > r.second;

        return l.first > r.first;
    });

    return vec;
}

std::vector<std::pair<std::string,float>> Policy::getGoalsProba()
{
    std::map<std::string, float> goalsProba = s.getGoalsProba();
    std::vector<std::pair<std::string,float>> vec;
    std::copy(goalsProba.begin(), goalsProba.end(), std::back_inserter<std::vector<std::pair<std::string,float>>>(vec));
    std::sort(vec.begin(), vec.end(), [](const std::pair<std::string,float>& l, const std::pair<std::string,float>& r) {
        if (l.second != r.second)
            return l.second > r.second;

        return l.first > r.first;
    });

    return vec;
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
