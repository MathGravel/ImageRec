#include <tuple>
#include "extendedPlanLibrary.h"
#include "bnflite.h"
#include "string"

using namespace bnf;

planLibrary currentPL = planLibrary();

std::unordered_set<int> prs_goals;
std::unordered_set<int> prs_terminals;
std::unordered_set<int> prs_nonTerminals;
std::unordered_map<int,rule> prs_rules;
std::map<string,int> prs_ids;
std::vector<std::tuple<string, int, float>> prs_dm;
std::vector<std::tuple<string, string, float>> prs_noise;

int currentSym;
int currentRule;
int tempConstraint;
string currentId;
string currentnSym;

static bool ar_goal(const char* lexem, size_t len)
{
    currentPL.addSymbol(currentSym,false,true);
    if(prs_ids.find(string(lexem,len)) == prs_ids.end())
    {
        prs_ids[string(lexem,len)]=currentSym;
        currentSym++;
    }
    return true;
}

static bool ar_terminals(const char* lexem, size_t len)
{
    currentPL.addSymbol(currentSym,true,false);
    if(prs_ids.find(string(lexem,len)) == prs_ids.end())
    {
        prs_ids[string(lexem,len)]=currentSym;
        currentSym++;
    }
    return true;
}

static bool ar_nonTerminals(const char* lexem, size_t len)
{
    currentPL.addSymbol(currentSym,false,false);
    if(prs_ids.find(string(lexem,len)) == prs_ids.end())
    {
        prs_ids[string(lexem,len)]=currentSym;
        currentSym++;
    }
    return true;
}

static bool ar_newId(const char* lexem, size_t len)
{
    currentRule = std::stoi(string(lexem,len));
    return true;
}

static bool ar_newRule(const char* lexem, size_t len)
{
    prs_rules[currentRule] = rule(prs_ids[string(lexem,len)],currentRule);
    return true;
}

static bool ar_newchild(const char* lexem, size_t len)
{
    prs_rules[currentRule].addChild(prs_ids[string(lexem,len)]);
    return true;
}

static bool ar_tempCst(const char* lexem, size_t len)
{
    tempConstraint = std::stoi(string(lexem,len));
    return true;
}

static bool ar_newCst(const char* lexem, size_t len)
{
    prs_rules[currentRule].addConstraint(std::make_pair(tempConstraint, std::stoi(string(lexem,len))));
    return true;
}

static bool ar_dmId(const char* lexem, size_t len)
{
    currentId = string(lexem,len);
    return true;
}

static bool ar_dmR(const char* lexem, size_t len)
{
    currentRule = std::stoi(string(lexem,len));
    return true;
}

static bool ar_dmProb(const char* lexem, size_t len)
{
    prs_dm.push_back(std::make_tuple(currentId,currentRule,std::stof(string(lexem,len)) ));
    return true;
}

static bool ar_noiseId(const char* lexem, size_t len)
{
    currentId = string(lexem,len);
    return true;
}

static bool ar_noiseSym(const char* lexem, size_t len)
{
    currentnSym = string(lexem,len);
    return true;
}

static bool ar_noiseProb(const char* lexem, size_t len)
{
    prs_noise.push_back(std::make_tuple(currentId, currentnSym ,std::stof(string(lexem,len)) ));
    return true;
}

extendedPlanLibrary::extendedPlanLibrary(const char* spl)
{

    currentPL = planLibrary();

    prs_goals = std::unordered_set<int>();
    prs_terminals = std::unordered_set<int>();
    prs_nonTerminals = std::unordered_set<int>();
    prs_rules = std::unordered_map<int,rule> ();
    prs_ids = std::map<string,int>();
    prs_ids["root"] = -1;

    currentSym = 0;
    currentRule = 0;
    tempConstraint = 0;

    Token digit('0','9');
    Token letter('a','z');
    letter.Add('_');
    letter.Add('A','Z');

    Lexem number = !Token("-") + 1*digit;
    Lexem val = !Token("-") + Token('0') + Token('.') +1*digit | Token('1') + Token('.') + 1*Token('0');
    Lexem name = 1*letter;

    Lexem goals = "Goals";
    Lexem terminals = "Terminals";
    Lexem nonTerminals = "NonTerminals";
    Lexem rules = "Rules";
    Lexem dm = "DecisionModel";
    Lexem nse = "Noise";
    Lexem delimiter = ",";
    Lexem newLine = "\n";

    Rule r_line_goals = goals + Token('(') + name + ar_goal + *(delimiter + name + ar_goal) + Token(')');
    Rule r_line_terminals = terminals + Token('(') + name + ar_terminals + *(delimiter + name + ar_terminals) + Token(')');
    Rule r_line_nonterminals = nonTerminals + Token('(') + name + ar_nonTerminals + *(delimiter + name + ar_nonTerminals) + Token(')');

    Rule r_rule_children = name + ar_newchild + *(delimiter + name+ ar_newchild );
    Rule r_rule_constraints = *(Token('(') + number + ar_tempCst + delimiter + number + ar_newCst +Token(')') + !delimiter);
    Rule r_rule = number + ar_newId + Token('(') + name + ar_newRule + Token('(') + r_rule_children + Token(')') + delimiter + Token('(') + r_rule_constraints + Token(')') + Token(')');
    Rule r_rules = rules + Token('(') + r_rule + *(delimiter + r_rule) + Token(')');

    Rule r_dm_prob = number + ar_dmR+ Token('(') + val + ar_dmProb + Token(')');
    Rule r_dm_Id = name + ar_dmId + Token('(') +  r_dm_prob + *(delimiter + r_dm_prob) + Token(')');
    Rule r_dm = dm +  Token('(') + r_dm_Id + *(delimiter + r_dm_Id) + Token(')');

    Rule r_noise_prob = name + ar_noiseSym + Token('(') + val + ar_noiseProb + Token(')');
    Rule r_noise_Id = name + ar_noiseId + Token('(') + r_noise_prob + *(delimiter + r_noise_prob) + Token(')');
    Rule r_noise = nse + Token('(') + r_noise_Id + *(delimiter + r_noise_Id) + Token(')');

    Rule root = r_line_goals + r_line_terminals + r_line_nonterminals + r_rules + !r_dm + !r_noise;

    const char* tail = 0;
    int tst = Analyze(root,spl,&tail);
    if (tst > 0)
        std::cout << "Plan Library configured"  << std::endl;
    else
        std::cout << "Parsing errors detected, status = " << tst << std::endl
<< "stopped at: " << tail << std::endl;

    for(auto it : prs_rules)
    {
        currentPL.addRule(it.second);
    }

    this->ids = prs_ids;
    for(auto it : this->ids)
    {
        this->revIds[it.second] = it.first;
    }
    this->ePlanLibrary = &currentPL;

    decisionModel = probabilityDistribution(*ePlanLibrary);

    for(auto it : prs_dm)
    {
        decisionModel.setProb(ids[get<0>(it)], get<1>(it), get<2>(it));
    }

    //add noise
    noise = probabilityDistribution(ePlanLibrary->getTerminals(), 0.0);
    for(auto it : prs_noise)
    {
        if(get<1>(it) == "miss")
        {
            noise.setProb(ids[get<0>(it)], -1, get<2>(it));
        }
        else if(get<1>(it) == "ext")
        {
            noise.setProb(ids[get<0>(it)], -2, get<2>(it));
        }
        else
            noise.setProb(ids[get<0>(it)], ids[get<1>(it)], get<2>(it));
    }

}

extendedPlanLibrary::extendedPlanLibrary(planLibrary* _ePlanLibrary)
{
    ePlanLibrary = _ePlanLibrary;
        //add DM
    decisionModel = probabilityDistribution(*ePlanLibrary);

    //add noise
    noise = probabilityDistribution(ePlanLibrary->getTerminals(), 0.0);
}


extendedPlanLibrary::extendedPlanLibrary(const extendedPlanLibrary& epl)
{
    //if((*this) != epl)
    //{
        delete ePlanLibrary;
        ePlanLibrary = epl.ePlanLibrary;
        decisionModel = epl.decisionModel;
        noise = epl.noise;
    //}
}

extendedPlanLibrary::extendedPlanLibrary(bool rnd, float noisePRCT , int goal,int _size, int depth, int widthAND , int widthOR  , bool DM )
{
    ePlanLibrary = new planLibrary();
    //keep a track of ids
    int idPrim = 0;
    int idRule = 0;
    char currentChar = 'a';
    string currentString = "G_";
    ids["root"] = -1;
    //assert(rnd);
    //add the control symbol for goals
    ePlanLibrary->addSymbol(-1,false,false);

    //add goals
    for(idPrim; idPrim < goal;idPrim++)
    {
        ePlanLibrary->addSymbol(idPrim,false,true);
        ids[currentString+currentChar]=idPrim;
        if(currentChar=='z')
        {
            currentChar = 'a';
            currentString = currentString + currentChar;
        }
        else
            currentChar++;

        rule g = rule(-1,idRule);
        g.addChild(idPrim);
        idRule++;
        ePlanLibrary->addRule(g);

        for(int j = 0; j < widthOR; j++)
        {
            rule r = rule(idPrim,idRule);
            idRule++;

            for(int k =0; k < widthAND; k++)
            {
                r.addChild(rand() % _size + _size);
                for(int rSym = 0; rSym < k; rSym++)
                {
                    if(rand() % 3 == 2)
                    {
                        r.addConstraint(std::make_pair(rSym,k));
                    }
                }
            }

            ePlanLibrary->addRule(r);
        }

    }
    //add non terminal symbols
    idPrim = _size;
    currentString = "S_";
    currentChar = 'a';
    for(int i =1; i<depth;i++)
    {
        for(idPrim; idPrim<_size*(i+1);idPrim++)
        {
            ePlanLibrary->addSymbol(idPrim,false,false);
            ids[currentString+currentChar]=idPrim;
            if(currentChar=='z')
            {
                currentChar = 'a';
                currentString = currentString + currentChar;
            }
            else
                currentChar++;


            for(int j = 0; j < widthOR; j++)
            {
                rule r = rule(idPrim,idRule);
                idRule++;

                for(int k=0; k < widthAND; k++)
                {
                    r.addChild(rand() % _size + _size*(i+1));
                    for(int rSym = 0; rSym < k; rSym++)
                    {
                        if(rand() % 3 == 2)
                        {
                            r.addConstraint(std::make_pair(rSym,k));
                        }
                    }
                }

                ePlanLibrary->addRule(r);
            }

        }
    }


    //add terminal symbols
    currentString = "A_";
    currentChar = 'a';
    for(idPrim;idPrim<_size*(depth+1);idPrim++)
    {
        ePlanLibrary->addSymbol(idPrim,true,false);
        ids[currentString+currentChar]=idPrim;
        if(currentChar=='z')
            {
                currentChar = 'a';
                currentString = currentString + currentChar;
            }
        else
            currentChar++;
    }

    //add DM
    decisionModel = probabilityDistribution(*ePlanLibrary);

    //add noise
    noise = probabilityDistribution(ePlanLibrary->getTerminals(), noisePRCT);

    for(auto it : ids)
    {
        revIds[it.second] = it.first;
    }

}

extendedPlanLibrary::~extendedPlanLibrary()
{
    //delete ePlanLibrary;
}

int extendedPlanLibrary::extra()
{
    int rnd = rand() % ePlanLibrary->getTerminals().size();
    auto it = ePlanLibrary->getTerminals().begin();
    for(int i = 0; i < rnd; i++)
    {
        ++it;
    }
    return *it;
} 

const string extendedPlanLibrary::toString()
{
    string resu = "";

    //add goals
    resu += "Goals(";
    int cnt = 0;
    for (auto itG : this->ePlanLibrary->getGoals())
    {
        cnt++;
        resu += this->revIds[itG];
        if(cnt < this->ePlanLibrary->getGoals().size())
        {
            resu+=",";
        }
    }
    resu+=") \n";

    //add terminals
    resu += "Terminals(";
    cnt = 0;
    for (auto itT : this->ePlanLibrary->getTerminals())
    {
        cnt++;
        resu += this->revIds[itT];
        if(cnt < this->ePlanLibrary->getTerminals().size())
        {
            resu+=",";
        }
    }
    resu+=") \n";

    //add non-terminals
    resu += "NonTerminals(";
    cnt = 0;
    for (auto itNT : this->ePlanLibrary->getNonTerminals())
    {
        cnt++;
        resu += this->revIds[itNT];
        if(cnt < this->ePlanLibrary->getNonTerminals().size())
        {
            resu+=",";
        }
    }
    resu+=") \n";

    //add rules
    resu += "Rules(";
    cnt = 0;
    for (auto itR : this->ePlanLibrary->getRules())
    {
        cnt++;
        resu += std::to_string(itR.first);
        resu+="(" + itR.second.toString(revIds) + ")";
        if(cnt < this->ePlanLibrary->getRules().size())
        {
            resu+=", \n";
        }
    }
    resu+=")";

    resu+=" \n";
    //add Dm
    cnt = 0;
    int cnt2;
    resu += "DecisionModel(";
    for (auto itDM : decisionModel.getDistribution())
    {
        cnt++;
        resu += revIds[itDM.first];
        resu += "(";
        cnt2 = 0;
        for (auto itSym : itDM.second)
        {
            cnt2++;
            resu += std::to_string(itSym.first) + "(" + std::to_string(itSym.second) +")";
            if(cnt2 < itDM.second.size())
            {
                resu+=",";
            }
        }

        resu += ")";
        if(cnt < decisionModel.getDistribution().size())
        {
            resu+=", \n";
        }
    }

    resu+=") \n";

    //add Noise
    cnt = 0;
    resu += "Noise(";
    for (auto itN : noise.getDistribution())
    {
        cnt++;
        resu += revIds[itN.first];
        resu += "(";
        cnt2 = 0;
        for (auto itSym : itN.second)
        {
            cnt2++;
            if(itSym.first == -1)
                resu +=  "miss(" + std::to_string(itSym.second) +")";
            else if(itSym.first == -2)
                resu +=  "ext(" + std::to_string(itSym.second) +")";
            else
                resu +=  revIds[itSym.first] + "(" + std::to_string(itSym.second) +")";
            if(cnt2 < itN.second.size())
            {
                resu+=",";
            }
        }

        resu += ")";
        if(cnt < noise.getDistribution().size())
        {
            resu+=", \n";
        }
    }
    resu+=") \n";

    return resu;
}

