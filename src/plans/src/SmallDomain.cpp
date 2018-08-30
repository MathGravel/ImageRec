#include <iostream>
#include "../include/Domain.h"
#include "../include/Generator.h"
#include "../include/Solver.h"
#include "../include/test.h"


using namespace std;

Domain SmallDomain::teaDomain()
{
    Domain domain = Domain();
    domain.addLiteral("hold(teakettle)",true,false);
    domain.addLiteral("hold(water)",true,false);
    domain.addLiteral("hold(mug)",true,false);
    domain.addLiteral("hold(tea)",true,false);
    domain.addLiteral("hold(pot)",true,false);
    domain.addLiteral("hold(coffe)",true,false);
    domain.addLiteral("hold(coffemaker)",true,false);
    domain.addLiteral("hold(milk)",true,false);
    domain.addLiteral("hold(choco)",true,false);

    domain.addLiteral("teamaking", false, true);
    domain.addLiteral("coffemaking", false, true);
    domain.addLiteral("chocomaking", false, true);

    std::vector<std::string> children = {"hold(teakettle)", "hold(water)", "hold(mug)", "hold(tea)"};
    std::vector<std::pair<std::string,std::string>> constraints = {make_pair("hold(teakettle)", "hold(water)"),make_pair("hold(water)", "hold(mug)"),make_pair("hold(water)", "hold(tea)")};
    int idProd = domain.addRule("teamaking", children, constraints,1.0);


    children = {"hold(teakettle)", "hold(water)", "hold(mug)", "hold(coffe)"};
    constraints = {make_pair("hold(teakettle)", "hold(water)"),make_pair("hold(water)", "hold(mug)"),make_pair("hold(mug)", "hold(coffe)")};
    idProd = domain.addRule("coffemaking", children, constraints, 0.25);

    children = {"hold(pot)", "hold(water)", "hold(mug)", "hold(coffe)"};
    constraints = {make_pair("hold(pot)", "hold(water)"),make_pair("hold(water)", "hold(mug)"),make_pair("hold(mug)", "hold(coffe)")};
    idProd = domain.addRule("coffemaking", children, constraints,0.25);

    children = {"hold(coffe)", "hold(coffemaker)", "hold(mug)", "hold(milk)"};
    constraints = {make_pair("hold(coffe)", "hold(coffemaker)"),make_pair("hold(coffemaker)", "hold(mug)"),make_pair("hold(mug)", "hold(milk)")};
    idProd = domain.addRule("coffemaking", children, constraints,0.25);

    children = {"hold(coffe)", "hold(coffemaker)", "hold(mug)"};
    constraints = {make_pair("hold(coffe)", "hold(coffemaker)"),make_pair("hold(coffemaker)", "hold(mug)")};
    idProd = domain.addRule("coffemaking", children, constraints, 0.25);

    children = {"hold(teakettle)", "hold(water)", "hold(mug)", "hold(choco)"};
    constraints = {make_pair("hold(teakettle)", "hold(water)"),make_pair("hold(water)", "hold(mug)"),make_pair("hold(mug)", "hold(choco)")};
    idProd = domain.addRule("chocomaking", children, constraints,0.5);

    children = {"hold(mug)", "hold(milk)", "hold(choco)"};
    constraints = {make_pair("hold(mug)", "hold(milk)"),make_pair("hold(milk)", "hold(choco)")};
    idProd = domain.addRule("chocomaking", children, constraints, 0.5);

    domain.setPriorProbability({make_pair("teamaking",0.25),make_pair("coffemaking",0.5),make_pair("chocomaking",0.25)});

    return domain;
}


Domain SmallDomain::getSmallDomain()
{
    Domain domain = Domain();
        domain.addLiteral("hold(teakettle)",true,false);
        domain.addLiteral("hold(water)",true,false);
        domain.addLiteral("hold(mug)",true,false);
        domain.addLiteral("hold(tea)",true,false);
        domain.addLiteral("hold(pot)",true,false);
        domain.addLiteral("hold(coffe)",true,false);
        domain.addLiteral("hold(coffemaker)",true,false);
        domain.addLiteral("hold(milk)",true,false);
        domain.addLiteral("hold(choco)",true,false);

        domain.addLiteral("teamaking", false, true);
        domain.addLiteral("coffemaking", false, true);
        domain.addLiteral("chocomaking", false, true);

        std::vector<std::string> children = {"hold(teakettle)", "hold(water)", "hold(mug)", "hold(tea)"};
        std::vector<std::pair<std::string,std::string>> constraints = {make_pair("hold(teakettle)", "hold(water)"),make_pair("hold(water)", "hold(mug)"),make_pair("hold(water)", "hold(tea)")};
        int idProd = domain.addRule("teamaking", children, constraints,1.0);


        children = {"hold(teakettle)", "hold(water)", "hold(mug)", "hold(coffe)"};
        constraints = {make_pair("hold(teakettle)", "hold(water)"),make_pair("hold(water)", "hold(mug)"),make_pair("hold(mug)", "hold(coffe)")};
        idProd = domain.addRule("coffemaking", children, constraints, 0.25);

        children = {"hold(pot)", "hold(water)", "hold(mug)", "hold(coffe)"};
        constraints = {make_pair("hold(pot)", "hold(water)"),make_pair("hold(water)", "hold(mug)"),make_pair("hold(mug)", "hold(coffe)")};
        idProd = domain.addRule("coffemaking", children, constraints,0.25);

        children = {"hold(coffe)", "hold(coffemaker)", "hold(mug)", "hold(milk)"};
        constraints = {make_pair("hold(coffe)", "hold(coffemaker)"),make_pair("hold(coffemaker)", "hold(mug)"),make_pair("hold(mug)", "hold(milk)")};
        idProd = domain.addRule("coffemaking", children, constraints,0.25);

        children = {"hold(coffe)", "hold(coffemaker)", "hold(mug)"};
        constraints = {make_pair("hold(coffe)", "hold(coffemaker)"),make_pair("hold(coffemaker)", "hold(mug)")};
        idProd = domain.addRule("coffemaking", children, constraints, 0.25);

        children = {"hold(teakettle)", "hold(water)", "hold(mug)", "hold(choco)"};
        constraints = {make_pair("hold(teakettle)", "hold(water)"),make_pair("hold(water)", "hold(mug)"),make_pair("hold(mug)", "hold(choco)")};
        idProd = domain.addRule("chocomaking", children, constraints,0.5);

        children = {"hold(mug)", "hold(milk)", "hold(choco)"};
        constraints = {make_pair("hold(mug)", "hold(milk)"),make_pair("hold(milk)", "hold(choco)")};
        idProd = domain.addRule("chocomaking", children, constraints, 0.5);

        domain.setPriorProbability({make_pair("teamaking",0.25),make_pair("coffemaking",0.5),make_pair("chocomaking",0.25)});

        return domain;
}

std::string SmallDomain::getMax(std::map<std::string, float> probDistribution)
{
    float maxValue =0.0;
    std::string resu;
    for(std::map<std::string, float>::const_iterator it = probDistribution.begin(); it != probDistribution.end(); it++)
    {
        if(it->second > maxValue)
        {
            maxValue = it->second;
            resu = it->first;
        }
    }
    return resu;
}


int SmallDomain::test()
{

        Domain d = teaDomain();
        Solver sol = Solver(d,1,200);
        sol.addObservation("hold(teakettle)");

        Testing thisTest = Testing(1);
        std::cout<<"Init Test"<<endl;
        map<int,int> goalCorrect;
        map<int,int> actionCorrect;
        for(int as = 0; as < 30;as++)
        {
            goalCorrect[as] = 0;
            actionCorrect[as] =0;
        }

        for(int d = 0; d < 1; d++)
        {
            Domain aDomain = thisTest.testDomain();
            for(int s = 0; s < 10; s++)
            {
                cout << std::to_string(d) + " - " + std::to_string(s)<<endl;
                Output aOutSequence = Output(&aDomain);
                while(aOutSequence.expand())
                {
                }
                Solver aSolver = Solver(aDomain,1,200);
                vector<int> aSequence = aOutSequence.getPOOutput();
                string goal = aDomain.getPlanLibrary().getLiteralName(aOutSequence.getGoals()[0]);

                for(int as =0; as <aSequence.size();as++)
                {
                    if(getMax(aSolver.getNextActions())==aDomain.getPlanLibrary().getLiteralName(aSequence[as]))
                        actionCorrect[as] +=1;
                    if(aSolver.addObservation(aSequence[as]))
                    {
                        if(getMax(aSolver.getGoalsProba()) == goal)
                            goalCorrect[as+1] +=1;
                    }
                    else
                        break;
                }
            }
        }

        for(map<int,int>::iterator it = goalCorrect.begin(); it != goalCorrect.end(); it++)
        {
            cout<< std::to_string(it->first) + " " + std::to_string(it->second)<<endl;
        }


        /*Domain aDomain = thisTest.testDomain();
        Solver aSolver = Solver(aDomain,1,1000);
        Output aSequence = Output(&aDomain);
        while(aSequence.expand())
        {
        }
        auto gp = aSolver.getGoalsProba();
        auto ap = aSolver.getNextActions();
        for(std::map<string,float>::const_iterator it = gp.begin(); it != gp.end(); ++it)
        {
            cout<< it->first;
            cout<< " ";
            cout<< it->second;
            cout<< " ";
        }
        cout<<endl;
        for(std::map<string,float>::const_iterator it = ap.begin(); it != ap.end(); ++it)
        {
            cout<< it->first;
            cout<< " ";
            cout<< it->second;
            cout<< " ";
        }
        cout<<endl;
        aSolver.addObservation(aSequence.getPOOutput()[0]);
        cout<< aSequence.getPOOutput()[0] <<endl;
        gp = aSolver.getGoalsProba();
        ap = aSolver.getNextActions();
        for(std::map<string,float>::const_iterator it = gp.begin(); it != gp.end(); ++it)
        {
            cout<< it->first;
            cout<< " ";
            cout<< it->second;
            cout<< " ";
        }
        cout<<endl;
        for(std::map<string,float>::const_iterator it = ap.begin(); it != ap.end(); ++it)
        {
            cout<< it->first;
            cout<< " ";
            cout<< it->second;
            cout<< " ";
        }
        cout<<endl;


        //thisTest.run();*/
        return 0;

}
