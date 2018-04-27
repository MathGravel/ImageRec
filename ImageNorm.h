//
// Created by uqamportable on 16/04/18.
// Cette classe sert a normaliser les resultats de predictions selon les informations du contexte
// a priori et selon son evolution au cours de l'execution de l'agent.
//

#ifndef IMAGEREC_IMAGENORM_H
#define IMAGEREC_IMAGENORM_H

#include <unordered_map>
#include <string>
#include <vector>



class ImageNorm {


public:
    ImageNorm();

    virtual ~ImageNorm();

    void updateContext(std::unordered_map<std::string,double> probs);
    void updateVisualContext(std::string objectUsed,std::vector<double> classProbs);
    std::pair<std::string,double> getCurrentObjectUsed();




};


#endif //IMAGEREC_IMAGENORM_H
