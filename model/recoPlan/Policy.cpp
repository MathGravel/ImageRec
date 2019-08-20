#include "Policy.h"

Policy::Policy() {

//this->load(pathDomain);

}

bool Policy::update(Affordance* observation)  {
        //std::cout << observation->getName() << std::endl;
    return gP.addObservation(observation->getName());
}

void Policy::Reset() {

      gP = solver(&ePL,500);
}


bool Policy::load(string pathDomain) {

	std::ifstream inputFile(pathDomain);
    // Check if exists and then open the file.
    if (inputFile.good()) {
        std::string temp;
        std::string insideText="";
        while (inputFile >> temp){
			insideText+=temp;
        }
        // Close the file.
        ePL = extendedPlanLibrary(insideText.c_str());
        //cout << ePL.toString()<<endl;
        gP = solver(&ePL,500);
        inputFile.close();
     }
     else {
        std::cerr << "Coulnd't open "<<pathDomain;
        exit(-1);
    }
}

bool comp(const pair<string, float>& a1, const pair<string, float>& a2)
{
    return a1.second < a2.second;
}

std::vector<std::pair<std::string,float>> Policy::getNextActions()
{
    auto nextAct = gP.getProbParticles();
    std::vector<std::pair<std::string,float>> vec;
    std::copy(nextAct.begin(), nextAct.end(), std::back_inserter<std::vector<std::pair<std::string,float>>>(vec));
    std::sort(vec.begin(), vec.end(), [](const std::pair<std::string,float>& l, const std::pair<std::string,float>& r) {
        if (l.second != r.second)
            return l.second > r.second;

        return l.first > r.first;
    });

    return vec;
}

std::vector<std::pair<std::string,float>> Policy::getGoalsProba()
{
    auto goalsProba = gP.getProbGoals();
    std::vector<std::pair<std::string,float>> vec;
    std::copy(goalsProba.begin(), goalsProba.end(), std::back_inserter<std::vector<std::pair<std::string,float>>>(vec));
    std::sort(vec.begin(), vec.end(), [](const std::pair<std::string,float>& l, const std::pair<std::string,float>& r) {
        if (l.second != r.second)
            return l.second > r.second;

        return l.first > r.first;
    });

    return vec;
}
