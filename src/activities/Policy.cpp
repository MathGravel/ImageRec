#include "Policy.h"

Policy::Policy() {

    this->load();

}

bool Policy::update(Affordance* observation)  {
    return gP.addObservation(observation->getName());
}

bool Policy::load() {


    QFile data(":/domains/smallDomain.txt");
    data.open(QIODevice::ReadOnly); //| QIODevice::Text)
    QTextStream in(&data);
    QString qs = in.readAll();
    std::string utf8_text = qs.toUtf8().constData();
    extendedPlanLibrary ePL = extendedPlanLibrary(utf8_text.c_str());
       cout << ePL.toString()<<endl;
      gP = solver(&ePL,500);
      data.close();
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
