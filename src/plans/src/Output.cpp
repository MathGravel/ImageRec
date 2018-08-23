#include "../include/Generator.h"
#include <utility>

Output::Output(Domain* domain)
{
	this->domain = domain;
	this->FOOutput = std::vector<int>();
    this->timeDurationOutput = std::vector<std::pair<float, float>>();
    this->POOutput = std::vector<int>();
    this->Goals = std::vector<int>();
    this->Goals.push_back(domain->getRandomGoal());
    this->treeOutput = OutputNode(this->Goals[0],this->domain);
    //std::cout<< domain->getRandomGoal()<<std::endl;
    //TODO :: multi-goal set

}

/*Output::Output(Output const &Output)
{

}*/

bool Output::expand()
{
    int newLit = this->treeOutput.expandOutput();
    //std::cout << "OutputNode Expand OK" << std::endl;
    if (newLit == -1)
    {
        return false;
    }
    this->FOOutput.push_back(newLit);
    //TODO timemanagement
    this->timeDurationOutput.push_back(std::make_pair(0.0,0.0));

    //random for POOutput
    std::random_device rd;
    std::mt19937 gen(rd());
    float rndValue = std::uniform_real_distribution<>(0.0, 1.0)(gen);



    //partial obs
    if (rndValue < this->domain->getObsRate())
    {
        rndValue = std::uniform_real_distribution<>(0.0, 1.0)(gen);
        if (rndValue < this->domain->getMutRate())
        {
            newLit = this->domain->getPlanLibrary().getRandomTerminal();
        }

        this->POOutput.push_back(newLit);
    }

    //addition
    rndValue = std::uniform_real_distribution<>(0.0, 1.0)(gen);
    if (rndValue < this->domain->getAddRate())
    {
        this->POOutput.push_back(this->domain->getPlanLibrary().getRandomTerminal());
    }


    return true;
}

