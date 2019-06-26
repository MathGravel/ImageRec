#ifndef OBJECTSMAT_H
#define OBJECTSMAT_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <unordered_map>


#include "DetectedObject.h"
#include "Affordance.h"

using namespace cv;
using namespace cv::dnn;

class ObjectsMat {
    const int MAXFRAMES = 20;
    const String NULLNAME = "NULL";
public:
    ObjectsMat() {
        mat_objets = {};
        //mat_mains = {};
        //vect_Frame = {};
    }

    void reset(){
        //this->mat_mains.clear();
        this->mat_objets.clear();
        //this->vect_Frame.clear();
    }

    void instance(){
        mat_objets = {};
        //mat_mains = {};
        //vect_Frame = {};
    }

    void update(std::vector<AffordanceTime*> objs/*const DetectedObjects& obj, const DetectedObjects& _hand, int frame*/){
        //mat_mains.push_back(_hand);
        mat_objets.push_back(objs);
        //vect_Frame.push_back(frame);

        while(this->mat_objets.size()>MAXFRAMES)
        {
           // this->mat_mains.erase(mat_mains.begin());
            this->mat_objets.erase(mat_objets.begin());
           // this->vect_Frame.erase(vect_Frame.begin());
        }
    }

    double getFrequence(String name)
    {   bool trouve=false;
        int nb=0;
        for(auto listAff : mat_objets)
        {
            trouve=false;
            for(auto item : listAff)
            {


            if(item->getAffordance().getName()==name && !trouve)
            {

                trouve = true;
                nb++;
                //nb=nb+0.75+(nb*item->getAffordance().getObjectProbability())*0.25;

            }
        }
        }
        if (mat_objets.size()>0)
            return (double)nb/(double)mat_objets.size();
        else return 0.5;
    }
/*
    std::vector<DetectedObjects> get_matObjects() const { return mat_objets;}
    std::vector<DetectedObjects> get_matMains() const { return mat_mains;}
    std::vector<int> get_vectFrame() const { return vect_Frame;}

*/
    std::vector<std::vector<AffordanceTime*>> get_affordances() const { return mat_objets;}
private:
    /*
    std::vector<DetectedObjects> mat_objets;
    std::vector<DetectedObjects> mat_mains;
    std::vector<int> vect_Frame;
*/
    std::vector<std::vector<AffordanceTime*>> mat_objets;



};
#endif //OBJECTSMAT_H
