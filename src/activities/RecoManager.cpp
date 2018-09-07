#include <RecoManager.h>
#include "RecoManager.h"

RecoManager::RecoManager(std::map<std::string,std::string> stream): trace(NULL) {

    act = ActivityRegion::instance();
    //act->deserialize(stream);
    this->deserialize(stream);
    //this->start_thread();

#ifdef USE_KITCHEN
    trace = new TraceManager("traces",this->feedSource.getScreenSize());
#endif

    informations = {
        {"actionPrecedente1", {{"nom", "NA"},{"pourcentage", "0"}}},
        {"actionPrecedente2", {{"nom", "NA"},{"pourcentage", "0"}}},
        {"actionActuelle", {{"nom", ""},{"pourcentage", "0"}}},
        {"planCourant1", {{"nom", "NA"},{"pourcentage", "0"}}},
        {"planCourant2", {{"nom", "NA"},{"pourcentage", "0"}}},
        {"planCourant3", {{"nom", "NA"},{"pourcentage", "0"}}},
        {"actionSuivante1", {{"nom", "NA"},{"pourcentage", "0"}}},
        {"actionSuivante2", {{"nom", "NA"},{"pourcentage", "0"}}},
        {"actionSuivante3", {{"nom", "NA"},{"pourcentage", "0"}}}
    };
}

RecoManager::~RecoManager() {
#ifdef USE_KITCHEN
    this->feedSource.saveVideos();
#endif
    this->reset();
    isStopped = false;
    if (trace != NULL)
        delete trace;
}

void RecoManager::update(){

    this->feedSource.update();
    colorPic =this->feedSource.getCurrentImage();
    depthPic = this->feedSource.getDepthImage();
    this->act->Update(this->feedSource.getOriginalImage(),this->feedSource.getDepthImage());
    this->feedSource.treatPicture(this->act);

}
void RecoManager::deserialize(std::map<std::string,std::string> stream){
    feedSource.deserialize(stream);
    act->deserialize(stream);
}

void RecoManager::saveVideos() {
    this->feedSource.saveVideos();
}

void RecoManager::reset(){
    isStopped = false;
    int i = 0;
    check.store(true);
    while (check.load() && (i < 100000))
        i++;
    act->reset();
#if USE_KITCHEN
    trace->dumpBuffer();
#endif
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
    string actionActuelleNomTemp = "";
    double actionActuellePourcentageTemp = 0;
    int actionActuelleCompteurTemp = 1;
    Domain domain = SmallDomain::getSmallDomain();
    //SmallDomain sm;
    //sm.test();
    Solver sol = Solver(domain,1,500);
    std::chrono::milliseconds startTime = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch());
    std::chrono::milliseconds actualTime = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch());


    //std::vector<std::pair<std::string,float>> tempActions =  sol.getNextActions();
    //---------------------
    std::map<std::string, float> nextActions = sol.getNextActions();
    std::vector<std::pair<std::string,float>> vec;
    std::copy(nextActions.begin(), nextActions.end(), std::back_inserter<std::vector<std::pair<std::string,float>>>(vec));
    std::sort(vec.begin(), vec.end(), [](const std::pair<std::string,float>& l, const std::pair<std::string,float>& r) {
        if (l.second != r.second)
            return l.second > r.second;

        return l.first > r.first;
    });
    std::vector<std::pair<std::string,float>> tempActions = vec;

    //---------------------
    //std::vector<std::pair<std::string,float>> tempGoal = sol.getGoalsProba();
    vec.clear();
    std::map<std::string, float> goalsProba = sol.getGoalsProba();
    std::copy(goalsProba.begin(), goalsProba.end(), std::back_inserter<std::vector<std::pair<std::string,float>>>(vec));
    std::sort(vec.begin(), vec.end(), [](const std::pair<std::string,float>& l, const std::pair<std::string,float>& r) {
        if (l.second != r.second)
            return l.second > r.second;

        return l.first > r.first;
    });
    std::vector<std::pair<std::string,float>> tempGoal = vec;

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
        actualTime = std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now().time_since_epoch());
        if(!act->currentAffordances.empty()){
            AffordanceTime* aff = act->currentAffordances.top();
            act->currentAffordances.pop();

            informations["actionActuelle"] = {{"nom",aff->getAffordance().getName()},{"pourcentage", to_string(aff->getAffordance().getObjectProbability()*100).substr(0,5)}};

                if (actionActuelleNom != aff->getAffordance().getName()) {
                    actionActuelleNom = aff->getAffordance().getName();
                    actionActuellePourcentage = aff->getAffordance().getObjectProbability()*100;
                    if (informations["actionPrecedente2"]["nom"] != actionActuelleNom) {
                        if (informations["actionPrecedente1"]["nom"] != actionActuelleNom) {
                            sol.addObservation("hold(" + aff->getAffordance().getName() + ")");
                            std::cout << "hold(" + aff->getAffordance().getName() + ")" << std::endl;

                            #ifdef USE_KITCHEN
                            trace->addAffordance(aff,(actualTime - startTime).count());
                            #endif
                        }
                        informations["actionPrecedente1"] = informations["actionPrecedente2"];
                        informations["actionPrecedente2"] = {{"nom", actionActuelleNom},{"pourcentage", to_string(actionActuellePourcentage).substr(0,5)}};
                    } else {
                        informations["actionPrecedente2"] = {{"nom", actionActuelleNom},{"pourcentage", to_string(actionActuellePourcentage).substr(0,5)}};
                    }
                }



            informations["actionActuelle"] = {{"nom",aff->getAffordance().getName()},{"pourcentage", to_string(aff->getAffordance().getObjectProbability()*100).substr(0,5)}};

            //std::vector<std::pair<std::string,float>> tempActions =  sol.getNextActions();
            //---------------------

            std::map<std::string, float> goalsProba = sol.getNextActions();

            vec.clear();
            std::copy(goalsProba.begin(), goalsProba.end(), std::back_inserter<std::vector<std::pair<std::string,float>>>(vec));
            std::sort(vec.begin(), vec.end(), [](const std::pair<std::string,float>& l, const std::pair<std::string,float>& r) {
                if (l.second != r.second)
                    return l.second > r.second;

                return l.first > r.first;
            });
            std::vector<std::pair<std::string,float>> tempActions = vec;

            //---------------------

           // std::vector<std::pair<std::string,float>> tempGoal = sol.getGoalsProba();

            //----------

            nextActions = sol.getGoalsProba();
            vec.clear();
            std::copy(nextActions.begin(), nextActions.end(), std::back_inserter<std::vector<std::pair<std::string,float>>>(vec));
            std::sort(vec.begin(), vec.end(), [](const std::pair<std::string,float>& l, const std::pair<std::string,float>& r) {
                if (l.second != r.second)
                    return l.second > r.second;

                return l.first > r.first;
            });
            std::vector<std::pair<std::string,float>> tempGoal = vec;
            //---------
            for(int i = 0; i < 3; i++) {
                std::string actS = "actionSuivante" + std::to_string(i+1);
                std::string plaS = "planCourant" + std::to_string(i+1);
                if (tempActions.size() > i) {
                    informations[actS] = {{"nom", (tempActions[i].first).erase(0,5)},{"pourcentage", to_string(tempActions[i].second).substr(0,5)}};
                    informations[actS]["nom"].pop_back();
                } else {
                    //informations[actS] = {{"nom", ""},{"pourcentage", "0"}};
                    informations[actS]["nom"] = "NA";
                    informations[actS]["pourcentage"] = "0";
                }
                if (tempGoal.size() > i) {
                    informations[plaS] = {{"nom", tempGoal[i].first},{"pourcentage", to_string(tempGoal[i].second).substr(0,5)}};
                } else {
                    //informations[plaS] = {{"nom", ""},{"pourcentage", "0"}};
                    informations[plaS]["nom"] = "NA";
                }


            }
            /*
            informations["planCourant2"] = {{"nom", tempGoal[1].first},{"pourcentage", to_string(tempGoal[1].second).substr(0,5)}};
            informations["planCourant3"] = {{"nom", tempGoal[2].first},{"pourcentage", to_string(tempGoal[2].second).substr(0,5)}};
                                                 ;

            informations["actionSuivante2"] = {{"nom", (tempActions[1].first).erase(0,5)},{"pourcentage", to_string(tempActions[1].second).substr(0,5)}};
            informations["actionSuivante2"]["nom"].pop_back();
            informations["actionSuivante3"] = {{"nom", (tempActions[2].first).erase(0,5)},{"pourcentage", to_string(tempActions[2].second).substr(0,5)}};
            informations["actionSuivante3"]["nom"].pop_back();
            */
            mtx.lock();
            mtx.unlock();

        }
#ifdef USE_KITCHEN
        else if (!act->AffordanceUpdated()) {
            int time = (actualTime - startTime).count();
            trace->addAffordance(NULL,time);
        }
#endif
        std::this_thread::sleep_for(std::chrono::microseconds(15));
    }

    std::cout << "Finished" << std::endl;
    check.store(false);
}
