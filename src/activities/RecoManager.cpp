#include "RecoManager.h"
#include <QDialog>;

#include <unistd.h>

#include <QMainWindow>
#include <QApplication>
#include <QStyle>
#include <QTranslator>
#include <QDesktopWidget>
#include <ctime>

const int TIME_LAST_AFFORDANCE=500;
RecoManager::RecoManager(std::map<std::string,std::string> stream): trace(NULL) {

    act = ActivityRegion::instance();
    //act->deserialize(stream);
    this->deserialize(stream);
    //this->start_thread();

    // Initialization of Policy and timer.
    pl= Policy();
    timeLectureStart=std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    timeSinceStart=std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
#ifdef USE_KITCHEN
    trace = new TraceManager("traces",this->feedSource.getScreenSize());
#endif
#ifdef USE_KITCHEN_DIST

    std::ifstream inputFile( "/home/troisiememathieu/Documents/codebaptiste/autre/reconnaissance-plans-activites/src/ressources/models/c-kitchen.name");

    // Check if exists and then open the file.
    if (inputFile.good()) {
        // Push items into a vector
        int nbClasses = 0;
        inputFile >> nbClasses;
        int i = 0;
        std::string temp;
        while (inputFile >> temp){
            objects[temp] = cv::Rect(0,0,0,0);
            i++;
        }
        // Close the file.
        inputFile.close();https://stackoverflow.com/questions/7889136/stdchrono-and-cout

    } else {
        std::cerr << "The classes names file is not available";
        exit(-1);
    }
    for (auto item : this->objects){
        names.push_back(item.first);
    }

    traceD = new TraceDistances("tracesDist",names,this->feedSource.getScreenSize());
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
#if defined(USE_KITCHEN) && !defined(USE_KITCHEN_DIST)
    this->feedSource.saveVideos();
#endif
    this->reset();
    isStopped = false;
    if (trace != NULL)
        delete trace;
}


void RecoManager::update(){
    timeSinceStart=std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    this->feedSource.update();
    colorPic =this->feedSource.getCurrentImage();
    depthPic = this->feedSource.getDepthImage();
    this->act->Update(this->feedSource.getOriginalImage(),this->feedSource.getDepthImage(),timeSinceStart.count()>TIME_LAST_AFFORDANCE);
//#ifndef USE_KITCHEN_DIST
    this->feedSource.treatPicture(this->act);
//#endif
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
#if USE_KITCHEN_DIST
    traceD->dumpBuffer();
    //delete traceD;
    //traceD = new TraceDistances("tracesDist",names,this->feedSource.getScreenSize());

#endif#include "NotifyMessageBox.h"
}


void RecoManager::start_thread(){
    std::thread affordanceThread(&RecoManager::start_affordance_check, this);
    affordanceThread.detach();
    th = &affordanceThread;
}

void RecoManager::start_affordance_check(){


        const int TIME_BEFORE_ALERT=30000;
        string actionActuelleNom = "";
        double actionActuellePourcentage = 0;
        int actionActuelleCompteur = 1;
        string actionActuelleNomTemp = "";
        double actionActuellePourcentageTemp = 0;
        int actionActuelleCompteurTemp = 1;







    std::chrono::milliseconds startTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    std::chrono::milliseconds actualTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());




        std::vector<std::pair<std::string,float>> tempActions = pl.getNextActions();
        std::vector<std::pair<std::string,float>> tempGoal = pl.getGoalsProba();

    while(isStopped){

        actualTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

        #ifndef USE_KITCHEN_DIST

        // if we have at least one affordance
        if(!act->currentAffordances.empty() ){

           Affordance* aff = ObjectsMat::updateAffordance(act->currentAffordances);


           if (actionActuelleNom != aff->getName() && aff->getName()!="NULL") {

                actionActuelleNom = aff->getName();
                actionActuellePourcentage = aff->getObjectProbability()*100;
                informations["actionActuelle"] = {{"nom",actionActuelleNom},{"pourcentage",  to_string(actionActuellePourcentage).substr(0,5)}};
                if (informations["actionPrecedente2"]["nom"] != actionActuelleNom){
                    if (informations["actionPrecedente1"]["nom"] != actionActuelleNom) {
                        pl.update(aff);

                        std::cout << "hold(" + aff->getName() + ")" << std::endl;
    #if defined(USE_KITCHEN)
                        trace->addAffordance(aff,(actualTime - startTime).count());
    #endif
                    }
                    informations["actionPrecedente1"] = informations["actionPrecedente2"];
                    informations["actionPrecedente2"] = {{"nom", actionActuelleNom},{"pourcentage", to_string(actionActuellePourcentage).substr(0,5)}};
                } else {
                    informations["actionPrecedente2"] = {{"nom", actionActuelleNom},{"pourcentage", to_string(actionActuellePourcentage).substr(0,5)}};
                }


                // Check if the new Action was plannedstartTime
                int i=0;
                if(!tempActions.empty()){

                while(i<tempActions.size() && tempActions[i].first!=aff->getName())
                {
                    i++;
                }

                    if(tempActions[i].first!=aff->getName())
                    {
                        std::cout<<"Error : actionNotPredict"<<std::endl;
                        informations["erreurPlan"] = {{"erreur", "You did an unexpected action :"+aff->getName()+" \nWe expected you to do : "+tempActions[0].first}};

                        startTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

                        pl = Policy();
                    }else{
                        std::cout<<"nouvelle action dans le plan de reconnaissance"<<std::endl;

                        informations["erreurPlan"] = {{"erreur", "Fine"}};


                    }
                }else{
                    std::cout<<"tempActions empty"<<std::endl;

                }
                tempActions = pl.getNextActions();
                tempGoal = pl.getGoalsProba();

                if(!tempGoal.empty()){
                    std::cout<<"tempGoal empty"<<std::endl;
                }
            }else{

                    if ((actualTime-startTime).count()<TIME_BEFORE_ALERT){

                        startTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
                        std::cout<<(actualTime-startTime).count()<<std::endl;
                        std::cout<<""<<std::endl;

                        if(!tempActions.empty()){
                            informations["erreurPlan"] = {{"erreur", "You didn't finish the goal :"+ tempGoal[0].first+" \nWe expected you to do : "+tempActions[0].first}};
                        }
                    }else{
                         informations["erreurPlan"] = {{"erreur", "Fine"}};
                    }
                }

            //---------
            for(int i = 0; i < 3; i++) {
                std::string actS = "actionSuivante" + std::to_string(i+1);
                std::string plaS = "planCourant" + std::to_string(i+1);
                if (tempActions.size() > i) {
                    informations[actS] = {{"nom", (tempActions[i].first)},{"pourcentage", to_string(tempActions[i].second).substr(0,5)}};
                    informations[actS]["nom"].pop_back();
    #ifdef USE_KITCHEN
                    std::string temp = informations[actS]["nom"] + " " + informations[actS]["pourcentage"];
                    trace->addFutureActivities(temp,i,(actualTime - startTime).count());
    #endif
                } else {
                    //informations[actS] = {{"nom", ""},{"pourcentage", "0"}};
                    informations[actS]["nom"] = "NA";
                    informations[actS]["pourcentage"] = "0";
                }
                if (tempGoal.size() > i) {
                    informations[plaS] = {{"nom", tempGoal[i].first},{"pourcentage", to_string(tempGoal[i].second).substr(0,5)}};
                    std::string temp = informations[plaS]["nom"] +" " + informations[plaS]["pourcentage"];
    #ifdef USE_KITCHEN
                    trace->addCurrentPlan(temp,(actualTime - startTime).count());
    #endif
                } else {
                    //informations[plaS] = {{"nom", ""},{"pourcentage", "0"}};
                    informations[plaS]["nom"] = "NA";
                }


            }

            mtx.lock();
            mtx.unlock();

        }

    #ifdef USE_KITCHEN
        else if (!act->AffordanceUpdated()) {
            int time = (actualTime - startTime).count();
            trace->addAffordance(NULL,time);
        }
    #endif

    #else
        mtx.lock();
        auto it = this->act->getItems();
        auto ma = this->act->getHands();
        for (auto obj : this->objects) {
            objects[obj.first] = cv::Rect(0,0,0,0);
        }

        for (auto item : it) {
            objects[item.getObjName()] = item.getObjPos();
        }

        for (auto obj : this->objects) {

            switch (ma.size())  {

                case 0 :
                    this->traceD->addHandDist(obj.first,-1,'A');
                break;
                case 1 :
            {
                    const cv::Rect posHand = ma.begin()->getObjPos();
                    const cv::Rect posItem = obj.second;
                    if (obj.second == cv::Rect(0,0,0,0)) {
                        this->traceD->addHandDist(obj.first,-1,'A');
                    } else {
                        double dist = cv::norm(cv::Point2d(posHand.x + posHand.width/2,posHand.y + posHand.height/2) -
                                               cv::Point2d(posItem.x + posItem.width/2,posItem.y + posItem.height/2));
                        this->traceD->addHandDist(obj.first,dist,'S');
                    }
            }
                break;

                case 2:
            {
                if (obj.second == cv::Rect(0,0,0,0)) {
                    this->traceD->addHandDist(obj.first,-1,'A');
                } else {
                     auto posHandL = ma.getObjects().front().getObjPos();
                     auto  posHandR = ma.getObjects().back().getObjPos();
                     const cv::Rect posItem = obj.second;

                    char hand = 'A';
                    if (posHandL.x < posHandR.x) {
                        auto posHandTemp = posHandR;
                        posHandR = posHandL;
                        posHandL = posHandTemp;
                    }

                    double distL = cv::norm(cv::Point2d(posHandL.x + posHandL.width/2,posHandL.y + posHandL.height/2) -
                                           cv::Point2d(posItem.x + posItem.width/2,posItem.y + posItem.height/2));
                    double distR = cv::norm(cv::Point2d(posHandR.x + posHandR.width/2,posHandR.y + posHandR.height/2) -
                                           cv::Point2d(posItem.x + posItem.width/2,posItem.y + posItem.height/2));
                    if (distL < distR)
                        this->traceD->addHandDist(obj.first,distL,'G');
                    else
                        this->traceD->addHandDist(obj.first,distR,'D');

                }
            }
                break;
            }

        }
        mtx.unlock();




    #endif
        std::this_thread::sleep_for(std::chrono::microseconds(1500));
    }

    std::cout << "Finished" << std::endl;
    check.store(false);
    }
