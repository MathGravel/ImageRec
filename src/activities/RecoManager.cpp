#include <RecoManager.h>
#include "RecoManager.h"

RecoManager::RecoManager(std::map<std::string,std::string> stream):feedSource(stream) {

    act = ActivityRegion::instance();
    act->deserialize(stream);
    this->deserialize(stream);
    this->start_thread();

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
    //std::thread affordanceThread(&RecoManager::start_affordance_check, this);
    //affordanceThread.detach();
    //th = &affordanceThread;
}

void RecoManager::start_affordance_check(){
    while(!isStopped.load()){
        if(!act->currentAffordances.empty()){
            AffordanceTime* aff = act->currentAffordances.top();
            act->currentAffordances.pop();

            std::string old_text ="";
            std::string old_prob = "";
            std::string test = aff->getAffordance().getName();

            pol.update(aff->getAffordance());



                std::stringstream ss;
                std::time_t t = std::time(0);   // get time now
                std::tm* now = std::localtime(&t);
                ss << (now->tm_year + 1900) << '-'
                   << (now->tm_mon + 1) << '-'
                   <<  now->tm_mday
                   << ' ' << now->tm_hour << ' ' << now->tm_min << ' ' << now->tm_sec  <<":\n";
                ss << "Current Action : " << aff->getAffordance().getName() << " " << floor(aff->getAffordance().getObjectProbability() * 100) << std::endl;
                ss << "Current Plan " << pol.getCurrentPlan() << " " << pol.getCurrentPlanProb() << std::endl;
                ss << "Next possible actions " << pol.getNextAction() << std::endl;

                std::cout << ss.str() << std::endl;


            std::string acts = pol.getNextAction();
            std::cout << acts << std::endl;

            mtx.lock();
            mtx.unlock();

        }
        std::this_thread::sleep_for(std::chrono::microseconds(15));
    }
}
