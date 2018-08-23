#include <iostream>
#include "../include/Domain.h"
#include "../include/Generator.h"
#include "../include/Solver.h"

using namespace std;


Domain smallDomain()
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


int test()
{
    Domain testD = teaDomain();
    Output Out = Output(&testD);

    Solver testS = Solver(testD,1,500);
    std::map<std::string, float> tempActions =  testS.getNextActions();
    std::map<std::string, float> tempGoal = testS.getGoalsProba();
    for(std::map<std::string, float>::iterator it = tempActions.begin();it != tempActions.end(); ++it)
    {
        cout<< it->first << " " << it->second << "|" ;
    }
    cout<<endl;
    cout << "********" << endl;
    for(std::map<std::string, float>::iterator it = tempGoal.begin();it != tempGoal.end(); ++it)
    {
        cout<< it->first << " " << it->second << "|" ;

    }
    cout<<endl;
     cout << "********" << endl;

    testS.addObservation("hold(teakettle)");
    tempActions =  testS.getNextActions();
    tempGoal = testS.getGoalsProba();

        for(std::map<std::string, float>::iterator it = tempActions.begin();it != tempActions.end(); ++it)
    {
        cout<< it->first << " " << it->second << "|" ;
    }
    cout<<endl;
    cout << "********" << endl;
    for(std::map<std::string, float>::iterator it = tempGoal.begin();it != tempGoal.end(); ++it)
    {
        cout<< it->first << " " << it->second << "|" ;

    }
    cout<<endl;
     cout << "********" << endl;

    cout << "Hello world!" << endl;
    return 0;
}
