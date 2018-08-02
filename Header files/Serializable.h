//
// Created by uqamportable on 02/08/18.
//

#ifndef IMAGEREC_SERIALIZABLE_H
#define IMAGEREC_SERIALIZABLE_H

#include <iostream>
#include <map>
#include <string>

class Serializable
{
public:
    virtual void deserialize(std::map<std::string,std::string> stream) = 0;
};

#endif //IMAGEREC_SERIALIZABLE_H
