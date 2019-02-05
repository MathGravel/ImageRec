#include "Tracemanager.h"

TraceManager::TraceManager(std::string traceLoc,std::pair<int,int> screen):fileLoc(std::move(traceLoc)),
    screenSize(screen)
{
    this->trace = std::ofstream(fileLoc + "/trace.txt");
    last = "";
    this->traceTime = std::ofstream(fileLoc + "/traceTime.txt");
}
TraceManager::~TraceManager(){
    if (this->trace.is_open()) {
        this->dumpBuffer();
    }


}

void TraceManager::addFrameCount(clock_t time){
    this->bufferTime << time << std::endl;
}
void TraceManager::addTotalCount(clock_t timeStart,clock_t timeEnd){
    this->bufferTime << "S " << timeStart << " E " << timeEnd  <<" Total duration : " << timeEnd - timeStart;
}


void TraceManager::dumpBuffer() {
    this->trace.write(buffer.str().data(), buffer.str().size());
    this->trace.close();
    buffer.clear();
    this->traceTime.write(this->bufferTime.str().data(),this->bufferTime.str().size());
    this->traceTime.close();
    this->bufferTime.clear();
}

void TraceManager::addAffordance(AffordanceTime* aff,int time){

    if (aff == NULL) {
        if (last == "Hand")
            return;
        else
            last = "Hand";
        buffer << time << " : Empty hands\n";
    } else {
        if (last == aff->getAffordance().getName())
            return;
        else
            last = aff->getAffordance().getName();
        buffer << time << ": "  << aff->getAffordance() <<
                  ((aff->getAffordance().getRegion().x < screenSize.first/2) ? " Left" : " Right") << std::endl;
    }
}
void TraceManager::addCurrentPlan(std::string plan,int time){

        buffer << time << ": "  << "Current plan : " <<
                  plan << std::endl;
}
void TraceManager::addFutureActivities(std::string ac1,int i,int time){
    buffer << time << ": "  << "Future activity " << i << " : " <<
              ac1 << std::endl;
}
