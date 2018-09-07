#include "Tracemanager.h"

TraceManager::TraceManager(std::string traceLoc,std::pair<int,int> screen):fileLoc(std::move(traceLoc)),
    screenSize(screen)
{
    this->trace = std::ofstream(fileLoc + "/trace.txt");
    last = "";
}
TraceManager::~TraceManager(){
    if (this->trace.is_open()) {
        this->dumpBuffer();
    }
}

void TraceManager::dumpBuffer() {
    this->trace.write(buffer.str().data(), buffer.str().size());
    this->trace.close();
    buffer.clear();
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
