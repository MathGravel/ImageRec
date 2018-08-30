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
    colorPic =this->feedSource.getCurrentImage();
    depthPic = this->feedSource.getDepthImage();
    this->act->Update(this->feedSource.getCurrentImage(),this->feedSource.getDepthImage());
    this->feedSource.treatPicture(this->act);
    //cv::imwrite("ttt.png",colorPic);

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

    string actionActuelleNom = "";
    double actionActuellePourcentage = 0;
    int actionActuelleCompteur = 1;

    std::vector<std::pair<std::string,float>> tempActions =  pol.getNextActions();
    std::vector<std::pair<std::string,float>> tempGoal = pol.getGoalsProba();
    informations["planCourant1"] = {{"nom", tempGoal[0].first},{"pourcentage", to_string(tempGoal[0].second).substr(0,5)}};
    informations["planCourant2"] = {{"nom", tempGoal[1].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
    informations["planCourant3"] = {{"nom", tempGoal[2].first},{"pourcentage", to_string(tempGoal[2].second).substr(0,5)}};
    informations["actionSuivante1"] = {{"nom", (tempActions[0].first).erase(0,5)},{"pourcentage", to_string(tempGoal[0].second).substr(0,5)}};
    informations["actionSuivante1"]["nom"].pop_back();
    informations["actionSuivante2"] = {{"nom", (tempActions[1].first).erase(0,5)},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
    informations["actionSuivante2"]["nom"].pop_back();
    informations["actionSuivante3"] = {{"nom", (tempActions[2].first).erase(0,5)},{"pourcentage", to_string(tempGoal[2].second).substr(0,5)}};
    informations["actionSuivante3"]["nom"].pop_back();

    while(isStopped){

        /*
        std::time_t t = std::time(0);
        std::tm* now = std::localtime(&t);

        cout << "**************** " << now->tm_mday << '/' << (now->tm_mon + 1) << '/' <<  (now->tm_year + 1900) << ':' << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << " ****************" << endl;
        cout << "Action Precedente 1 : " << informations["actionPrecedente1"]["nom"] << " - " << informations["actionPrecedente1"]["pourcentage"] << endl;
        cout << "Action Precedente 2 : " << informations["actionPrecedente2"]["nom"] << " - " << informations["actionPrecedente2"]["pourcentage"] << endl;
        cout << "Action Actuelle : " << informations["actionActuelle"]["nom"] << " - " << informations["actionActuelle"]["pourcentage"] << endl;
        cout << "Plan Courant 1 : " << informations["planCourant1"]["nom"] << " - " << informations["planCourant1"]["pourcentage"] << endl;
        cout << "Plan Courant 2 : " << informations["planCourant2"]["nom"] << " - " << informations["planCourant2"]["pourcentage"] << endl;
        cout << "Plan Courant 3 : " << informations["planCourant3"]["nom"] << " - " << informations["planCourant3"]["pourcentage"] << endl;
        cout << "Action Suivante 1 : " << informations["actionSuivante1"]["nom"] << " - " << informations["actionSuivante1"]["pourcentage"] << endl;
        cout << "Action Suivante 2 : " << informations["actionSuivante2"]["nom"] << " - " << informations["actionSuivante2"]["pourcentage"] << endl;
        cout << "Action Suivante 3 : " << informations["actionSuivante3"]["nom"] << " - " << informations["actionSuivante3"]["pourcentage"] << endl;
        cout << "***************************************************" << endl << endl;
        */

        if(!act->currentAffordances.empty()){
            AffordanceTime* aff = act->currentAffordances.top();
            act->currentAffordances.pop();

            if (actionActuelleNom == aff->getAffordance().getName()) {

                if (actionActuellePourcentage < (aff->getAffordance().getObjectProbability()*100)) {
                    actionActuellePourcentage = aff->getAffordance().getObjectProbability()*100;
                }
                actionActuelleCompteur++;

            } else {

                if (actionActuelleCompteur > 8) {

                    if (informations["actionPrecedente2"]["nom"] != actionActuelleNom) {
                        //pol.update(aff->getAffordance());

                        informations["actionPrecedente1"] = informations["actionPrecedente2"];
                        informations["actionPrecedente2"] = {{"nom", actionActuelleNom},{"pourcentage", to_string(actionActuellePourcentage).substr(0,5)}};
                    } else {
                        informations["actionPrecedente2"] = {{"nom", actionActuelleNom},{"pourcentage", to_string(actionActuellePourcentage).substr(0,5)}};
                    }
                }

                actionActuelleNom = aff->getAffordance().getName();
                actionActuellePourcentage = aff->getAffordance().getObjectProbability()*100;
                actionActuelleCompteur = 1;
            }

            informations["actionActuelle"] = {{"nom", aff->getAffordance().getName()},{"pourcentage", to_string(aff->getAffordance().getObjectProbability()*100).substr(0,5)}};

            std::vector<std::pair<std::string,float>> tempActions =  pol.getNextActions();
            std::vector<std::pair<std::string,float>> tempGoal = pol.getGoalsProba();
            informations["planCourant1"] = {{"nom", tempGoal[0].first},{"pourcentage", to_string(tempGoal[0].second).substr(0,5)}};
            informations["planCourant2"] = {{"nom", tempGoal[1].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
            informations["planCourant3"] = {{"nom", tempGoal[2].first},{"pourcentage", to_string(tempGoal[2].second).substr(0,5)}};
            informations["actionSuivante1"] = {{"nom", (tempActions[0].first).erase(0,5)},{"pourcentage", to_string(tempGoal[0].second).substr(0,5)}};
            informations["actionSuivante1"]["nom"].pop_back();
            informations["actionSuivante2"] = {{"nom", (tempActions[1].first).erase(0,5)},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
            informations["actionSuivante2"]["nom"].pop_back();
            informations["actionSuivante3"] = {{"nom", (tempActions[2].first).erase(0,5)},{"pourcentage", to_string(tempGoal[2].second).substr(0,5)}};
            informations["actionSuivante3"]["nom"].pop_back();

            mtx.lock();
            mtx.unlock();

        }
        std::this_thread::sleep_for(std::chrono::microseconds(15));
    }

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
