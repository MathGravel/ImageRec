#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

using namespace std;

#ifndef RULE_H
#define RULE_H

class rule
{
    public:
        rule();
        rule(int _primitive, int _id);
        rule(int _id, string s);
        //rule(const rule& _rule);
        virtual ~rule();

        void addChild(int child);
        void addConstraint(pair<int,int> constraint);
        void addChildren(vector<int> _children);
        void addConstraints(vector<pair<int,int>> _constraints);

        bool operator<(const rule _r) const;
        bool operator==(const rule _r) const;
        operator int() const;

        const int getPrimitive() const;
        const vector<int>& getChildren() const;
        const vector<pair<int,int>>& getConstraints() const;

        const string toString();
        const string toString(map<int,string> revIds);

        friend std::ostream& operator << (std::ostream&, const rule&);

    private:
        int primitive;
        int id;
        vector<int> children;
        vector<pair<int,int>> constraints;

};

namespace std{
template<>
    struct hash<rule>
    {
      size_t operator()(const rule & obj) const
      {
        return hash<int>()((int)obj);
      }
};
}

#endif // RULE_H
