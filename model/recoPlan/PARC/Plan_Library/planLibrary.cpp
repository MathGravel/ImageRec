#include "../include/planLibrary.h"
#include "../include/bnflite.h"

using namespace bnf;

/*std::unordered_set<int> prs_goals;
std::unordered_set<int> prs_terminals;
std::unordered_set<int> prs_nonTerminals;
std::unordered_map<int,rule> prs_rules;

int currentRule;
int tempConstraint;

static bool ar_goal(const char* lexem, size_t len)
{
    prs_goals.insert(std::stoi(string(lexem,len)));
    return true;
}

static bool ar_terminals(const char* lexem, size_t len)
{
    prs_terminals.insert(std::stoi(string(lexem,len)));
    return true;
}

static bool ar_nonTerminals(const char* lexem, size_t len)
{
    prs_nonTerminals.insert(std::stoi(string(lexem,len)));
    return true;
}

static bool ar_newId(const char* lexem, size_t len)
{
    currentRule = std::stoi(string(lexem,len));
    return true;
}

static bool ar_newRule(const char* lexem, size_t len)
{
    prs_rules[currentRule] = rule(std::stoi(string(lexem,len)),currentRule);
    return true;
}

static bool ar_newchild(const char* lexem, size_t len)
{
    prs_rules[currentRule].addChild(std::stoi(string(lexem,len)));
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
}*/

planLibrary::planLibrary()
{
    //ctor
}
/*
planLibrary::planLibrary(const char* spl)
{
    Token digit('0','9');

    Lexem number = !Token("-") + 1*digit;
    Lexem goals = "Goals";
    Lexem terminals = "Terminals";
    Lexem nonTerminals = "NonTerminals";
    Lexem rules = "Rules";
    Lexem delimiter = ",";
    Lexem newLine = "\n";

    Rule r_line_goals = goals + Token('(') + number + ar_goal + *(delimiter + number + ar_goal) + Token(')');
    Rule r_line_terminals = terminals + Token('(') + number + ar_terminals + *(delimiter + number + ar_terminals) + Token(')');
    Rule r_line_nonterminals = nonTerminals + Token('(') + number + ar_nonTerminals + *(delimiter + number + ar_nonTerminals) + Token(')');

    Rule r_rule_children = number + ar_newchild + *(delimiter + number+ ar_newchild );
    Rule r_rule_constraints = *(Token('(') + number + ar_tempCst + delimiter + number + ar_newCst +Token(')') + !delimiter);
    Rule r_rule = number + ar_newId + Token('(') + number + ar_newRule + Token('(') + r_rule_children + Token(')') + delimiter + Token('(') + r_rule_constraints + Token(')') + Token(')');
    Rule r_rules = rules + Token('(') + *(r_rule) + Token(')');

    Rule root = r_line_goals + r_line_terminals + r_line_nonterminals + r_rules;

    const char* tail = 0;
    int tst = Analyze(root,spl,&tail);
    if (tst > 0)
        std::cout << "Plan Libraty configured"  << std::endl;
    else
        std::cout << "Parsing errors detected, status = " << tst << std::endl
<< "stopped at: " << tail << std::endl;

    this->goals = prs_goals;
    this->terminals = prs_terminals;
    this->nonTerminals = prs_nonTerminals;
    this->rules = prs_rules;

}*/

planLibrary::~planLibrary()
{
    //dtor
}

bool planLibrary::isTerminal(int symbol) const
{
    return terminals.count(symbol) > 0;
}

bool planLibrary::isGoal(int symbol) const
{
    return goals.count(symbol) > 0;
}

int planLibrary::addRule(rule r)
{
    rules[(int)r] = r;
    if(nonTerminals.count(r.getPrimitive()) == 0)
        nonTerminals.insert(r.getPrimitive());
    for(auto it : r.getChildren())
    {
        if(nonTerminals.count(it) == 0)
            if(terminals.count(it) == 0)
                nonTerminals.insert(it);
    }
}

int planLibrary::addSymbol(int s, bool t, bool g)
{
    if(t)
        terminals.insert(s);
    else
        nonTerminals.insert(s);
    if(g)
        goals.insert(s);
}

const string planLibrary::toString()
{
    string resu = "";

    //add goals
    resu += "Goals(";
    int cnt = 0;
    for (auto itG : this->goals)
    {
        cnt++;
        resu += std::to_string(itG);
        if(cnt < this->goals.size())
        {
            resu+=",";
        }
    }
    resu+=") \n";

    //add terminals
    resu += "Terminals(";
    cnt = 0;
    for (auto itT : this->terminals)
    {
        cnt++;
        resu += std::to_string(itT);
        if(cnt < this->terminals.size())
        {
            resu+=",";
        }
    }
    resu+=") \n";

    //add non-terminals
    resu += "NonTerminals(";
    cnt = 0;
    for (auto itNT : this->nonTerminals)
    {
        cnt++;
        resu += std::to_string(itNT);
        if(cnt < this->nonTerminals.size())
        {
            resu+=",";
        }
    }
    resu+=") \n";

    //add rules
    resu += "Rules(";
    cnt = 0;
    for (auto itR : this->rules)
    {
        cnt++;
        resu += std::to_string(itR.first);
        resu+="(" + itR.second.toString() + ")";
        if(cnt < this->rules.size())
        {
            resu+=" \n";
        }
    }
    resu+=")";

    return resu;
}

const unordered_set<int>& planLibrary::getTerminals() const
{
    return terminals;
}

const unordered_set<int>& planLibrary::getNonTerminals() const
{
    return nonTerminals;
}

const unordered_set<int>& planLibrary::getGoals() const
{
    return goals;
}

const unordered_map<int,rule>& planLibrary::getRules() const
{
    return rules;
}
