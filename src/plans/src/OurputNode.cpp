#include "../include/Generator.h"

OutputNode::OutputNode()
{
    this->listChildren = std::vector<OutputNode> ();
    this->literal = -1;
    this->productionRule = ProductionRule();
    this->finish = true;
    //std::cout << this->literal << " A"<< std::endl;
}

OutputNode::OutputNode(int literal, Domain* domain)
{
    this->listChildren = std::vector<OutputNode> ();
    this->literal = literal;
    this->productionRule = domain->getRandomProduction(literal);
    this->finish = false;
    this->domain = domain;
    //std::cout << this->literal << " B"<< std::endl;
}

OutputNode::OutputNode(OutputNode const &OutputNode)
{
    this->listChildren = OutputNode.getChildren();
    this->literal = OutputNode.getLiteral();
    this->productionRule = OutputNode.getProduction();
    this->finish = OutputNode.isFinished();
    this->domain = OutputNode.domain;
    //std::cout << this->literal << " C"<< std::endl;
}

int OutputNode::expandOutput()
{
    if(this->domain->getPlanLibrary().isTerminal(this->literal))
    {
        this->finish = true;
        return this->literal;
    }
    //else recursion on children
    if(this->finish)
    {
        return -1;
    }
    int out = this->selectValidSymbol()->expandOutput();

    this->finish =true;
    for(std::vector<OutputNode>::const_iterator it = this->listChildren.begin(); it != this->listChildren.end(); ++it)
    {
        if(!it->isFinished())
        {
            this->finish = false;
            break;
        }
    }

    return out;
}

OutputNode* OutputNode::selectValidSymbol()
{
    if(this->listChildren.size() == 0)
    {
        //std::cout<< "construct children" <<std::endl;
        std::unordered_set<int> temp = this->productionRule.getChildren();
        for(std::unordered_set<int>::iterator it = temp.begin(); it != temp.end(); ++it)
        {
            this->listChildren.push_back(OutputNode(*it,this->domain));
        }
    }
    //std::cout << "listChildren Init OK" << std::endl;
    std::vector<OutputNode*> listCandidate = std::vector<OutputNode*> ();
    for(int it =0; it < this->listChildren.size(); it++)
    {
        if(!this->listChildren[it].isFinished())
        {
            listCandidate.push_back(&listChildren[it]);
        }
    }
    //std::cout << "listCandidate Init OK" << std::endl;

    while(!listCandidate.empty())
    {
        int randomIndex = rand() % listCandidate.size();
        bool isValid = true;

        std::vector<int> temp = this->productionRule.getConstraints()[listCandidate[randomIndex]->getLiteral()];

        for(std::vector<int>::iterator it = temp.begin(); it!=temp.end();++it)
        {
            if(!this->isValid(*it))
            {
                isValid = false;
                break;
            }
        }
        if (isValid)
            return listCandidate[randomIndex];
        else
            listCandidate.erase(listCandidate.begin()+randomIndex);
    }
    std::cout << "There is a problem here" << std::endl;
}

bool OutputNode::isValid (int lit) const
{
    for(std::vector<OutputNode>::const_iterator it = this->listChildren.begin(); it != this->listChildren.end(); ++it)
    {
        //std::cout << it->getLiteral() << " "<< lit << std::endl;
        if(it->getLiteral() == lit)
        {
            if(it->isFinished())
                return true;
            else
                return false;
        }
    }
    return false;
}


