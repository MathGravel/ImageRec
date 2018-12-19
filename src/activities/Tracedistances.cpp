#include "Tracedistances.h"

TraceDistances::TraceDistances(std::string traceLoc,std::vector<std::string> classes,std::pair<int,int> screen):fileLoc(std::move(traceLoc)),
    screenSize(screen)
{
    for (auto names : classes ) {
        std::string name = names;
        remove_if(name.begin(), name.end(), isspace);
        this->trace[names] = std::string(fileLoc + "/" +name + ".txt");
        this->buffer[names] = std::stringstream();
    }

}


TraceDistances::~TraceDistances(){
    //if (this->buffer.begin()->second.is_open()) {
     //   this->dumpBuffer();
    //}

}

void TraceDistances::addHandDist(std::string classe,double distance, char hand) {
    this->buffer[classe] << distance << " " << hand << std::endl;
}


void TraceDistances::dumpBuffer() {

    std::map<std::string,std::stringstream>::iterator itBuf = buffer.begin();
    std::map<std::string,std::string>::iterator itTr = trace.begin();

    while (itBuf != buffer.end()) {
        std::ofstream file(itTr->second);
        file.write(itBuf->second.str().data(), itBuf->second.str().size());
        file.close();
        itBuf->second.clear();
        itTr++;
        itBuf++;
    }


}
