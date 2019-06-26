#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <iostream>
#include <map>
#include <string>

class Serializable
{
public:
    virtual void deserialize(std::map<std::string,std::string> stream) = 0;
};

#endif //SERIALIZABLE_H
