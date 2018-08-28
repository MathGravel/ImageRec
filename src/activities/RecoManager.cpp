#include <RecoManager.h>
#include "RecoManager.h"

RecoManager::RecoManager(std::map<std::string,std::string> stream):feedSource(stream) {

    act = ActivityRegion::instance();
    act->deserialize(stream);
    this->deserialize(stream);
    //this->start_thread();

    informations = {
        {"actionPrecedente1", {{"nom", ""},{"pourcentage", "0"}}},
        {"actionPrecedente2", {{"nom", ""},{"pourcentage", "0"}}},
        {"actionActuelle", {{"nom", ""},{"pourcentage", "0"}}},
        {"planCourant1", {{"nom", ""},{"pourcentage", "0"}}},
        {"planCourant2", {{"nom", ""},{"pourcentage", "0"}}},
        {"planCourant3", {{"nom", ""},{"pourcentage", "0"}}},
        {"actionSuivante1", {{"nom", ""},{"pourcentage", "0"}}},
        {"actionSuivante2", {{"nom", ""},{"pourcentage", "0"}}},
        {"actionSuivante3", {{"nom", ""},{"pourcentage", "0"}}}
    };
}

RecoManager::~RecoManager() {
    delete act;
}

void RecoManager::update(){
    this->feedSource.update();
    this->act->Update(this->feedSource.getCurrentImage(),this->feedSource.getDepthImage());
    this->feedSource.treatPicture(this->act);
    colorPic =this->feedSource.getCurrentImage();
    depthPic = this->feedSource.getDepthImage();
    this->updatePolicy();

}
void RecoManager::deserialize(std::map<std::string,std::string> stream){
    feedSource.deserialize(stream);
    act->deserialize(stream);
}

void RecoManager::updatePolicy() {

}

void RecoManager::start_thread(){
    std::thread affordanceThread(&RecoManager::start_affordance_check, this);
    affordanceThread.detach();
    th = &affordanceThread;
}

void RecoManager::start_affordance_check(){
    while(!isStopped){

        std::vector<std::pair<std::string,float>> tempActions =  pol.getNextActions();
        std::vector<std::pair<std::string,float>> tempGoal = pol.getGoalsProba();
        informations["planCourant1"] = {{"nom", tempGoal[0].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
        informations["planCourant2"] = {{"nom", tempGoal[1].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
        informations["planCourant3"] = {{"nom", tempGoal[2].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
        informations["actionSuivante1"] = {{"nom", tempActions[0].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
        informations["actionSuivante2"] = {{"nom", tempActions[1].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
        informations["actionSuivante3"] = {{"nom", tempActions[2].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};

        if(!act->currentAffordances.empty()){
            AffordanceTime* aff = act->currentAffordances.top();
            act->currentAffordances.pop();

            informations["actionActuelle"] = {{"nom", aff->getAffordance().getName()},{"pourcentage", to_string(aff->getAffordance().getObjectProbability()*100).substr(0,5)}};
            pol.update(aff->getAffordance());



            cout << informations["actionActuelle"]["nom"] << " - " << informations["actionActuelle"]["pourcentage"] << endl;


/*
            std::string old_text ="";
            std::string old_prob = "";
            std::string test = aff->getAffordance().getName();

                std::stringstream ss;
                std::time_t t = std::time(0);   // get time now
                std::tm* now = std::localtime(&t);
                ss << (now->tm_year + 1900) << '-'
                   << (now->tm_mon + 1) << '-'
                   <<  now->tm_mday
                   << ' ' << now->tm_hour << ' ' << now->tm_min << ' ' << now->tm_sec  <<":\n";
                ss << "Current Action : " << aff->getAffordance().getName() << " " << floor(aff->getAffordance().getObjectProbability() * 100) << std::endl;



*/




                std::vector<std::pair<std::string,float>> tempActions =  pol.getNextActions();
                std::vector<std::pair<std::string,float>> tempGoal = pol.getGoalsProba();
                informations["planCourant1"] = {{"nom", tempGoal[0].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
                informations["planCourant2"] = {{"nom", tempGoal[1].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
                informations["planCourant3"] = {{"nom", tempGoal[2].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
                informations["actionSuivante1"] = {{"nom", tempActions[0].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
                informations["actionSuivante2"] = {{"nom", tempActions[1].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
                informations["actionSuivante3"] = {{"nom", tempActions[2].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};

            mtx.lock();
            mtx.unlock();

        }
        std::this_thread::sleep_for(std::chrono::microseconds(15));
    }
    terminate();
}
