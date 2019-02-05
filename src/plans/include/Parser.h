#ifndef PARSER_H
#define PARSER_H

#include<string>
#include<iostream>
#include<fstream>
#include<assert.h>
#include<set>
#include<vector>
#include <sstream>      // std::istringstream
#include <utility>
#include"Domain.h"
#include <algorithm>

class Parser
{
    public:
        Parser(std::string languageLoc = "");
        virtual ~Parser();


        Domain parseLanguage();
        void setLanguage(std::string languageLoc);

    protected:

        void parseNoise(std::ifstream& lng,Domain& dom);
        void parseObservables(std::ifstream& lng);
        void parsePrimitives(std::ifstream& lng,Domain& dom);
        void parseActions(std::ifstream& lng,Domain& dom);
        void parseGoals(std::ifstream& lng,Domain& dom);
        void createLitterals(Domain& dom);


    private:
    std::string permuteLitterals(std::set<std::string> UnusedLitt, std::string currentLitt,int neededLit);


    class Primitives {
        public:
            Primitives():nbVariables(0) {}
            Primitives(std::string n,int nbVar,ActionTime act) : name(n),nbVariables(nbVar),time(act){}
            std::string name;
            int nbVariables;
            ActionTime time;
    };
    std::string language;
    std::string delimiters = "()<>,";
    std::ifstream lngInput;
    double oRate = 1.0;
    double mRate = 0.0;
    double aRate = 0.0;
    std::set<std::string> observables;
    std::vector<Primitives> prim;




};

#endif // PARSER_H
