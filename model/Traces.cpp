#include "Traces.h"

Traces::Traces(std::string traceLoc,std::string planName):fileLoc(std::move(traceLoc))
{
	std::string direct;
	for (int i = 0; i < 4;i++)
		planName.pop_back();
  std::size_t found = planName.find_last_of("/\\");
  planName = planName.substr(found+1);
  direct = fileLoc + "/" + planName + ".txt";
  while (exists_test0(direct)) {
	for (int i = 0; i < 4;i++)
		direct.pop_back();
	direct = direct + "(a).txt";
  }
    this->trace = std::ofstream(direct);
    std::cout <<"\n\n\n\n\nICITTE :" << direct << std::endl;
    last = "";
    buffer << planName << std::endl;
}
Traces::~Traces(){
    if (this->trace.is_open()) {
        this->dumpBuffer();
    }
}

void Traces::addPlanReset() {
	buffer << "Reset" << std::endl;
}
inline bool Traces::exists_test0 (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}



void Traces::dumpBuffer() {
    this->trace.write(buffer.str().data(), buffer.str().size());
    this->trace.close();
    buffer.clear();
}

void Traces::addAffordance(Affordance* aff){

    buffer << "1 : "  << aff->getName() << std::endl;

}
void Traces::addCurrentPlan(std::string plan){

        buffer  << "2 : "   <<
                  plan << std::endl;
}
void Traces::addPossiblePlans(std::string plan){

        buffer << ": "  <<
                  plan << std::endl;
}

void Traces::addFutureActivities(std::string ac1){
    buffer  << "3 : "  <<
              ac1 << std::endl;
}
