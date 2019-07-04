#ifndef OBJECTSMAT_H
#define OBJECTSMAT_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

#include <vector>
#include <unordered_map>
#include <stack>

#include "DetectedObject.h"
#include "Affordance.h"


#include "Policy.h"
#include <algorithm>

using namespace cv;
using namespace cv::dnn;

class ObjectsMat {
    //const int MAXFRAMES = 20;
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

    static Affordance* updateAffordance(const std::stack<AffordanceTime*> matrice){
        vector<String> nameObject={};
        vector<double> probObject={};
        int j;
        std::stack<AffordanceTime*> allAff= matrice;
        AffordanceTime* searchAff;
        int rang;
        for(int i=0;i<std::min((int)matrice.size(),10);i++){
            searchAff=allAff.top();
            allAff.pop();
            j=0;
            rang = -1;
            while(j<nameObject.size()){
                if( nameObject.at(j)==searchAff->getAffordance().getName()){
                    rang=j;
                }
                j++;

            }

            if(rang !=-1 && nameObject[rang]==searchAff->getAffordance().getName()){
                probObject[rang]=probObject[rang]+searchAff->getAffordance().getObjectProbability();
            }
            else{
                nameObject.push_back(searchAff->getAffordance().getName());
                probObject.push_back(searchAff->getAffordance().getObjectProbability());
            }
        }
        // On a vérifié que currentAffordances != empty

        int rangMax=0;
        for(int a=1;a<probObject.size();a++){
            if(probObject[rangMax]<probObject[a]){
                rangMax=a;
            }
        }
        return new Affordance(nameObject[rangMax],0,cv::Rect(),probObject[rangMax]/std::min((int)matrice.size(),10),0);
    }

    void update(std::vector<AffordanceTime*> objs, bool supAtime/*const DetectedObjects& obj, const DetectedObjects& _hand, int frame*/){
        //mat_mains.push_back(_hand);
        mat_objets.push_back(objs);
        //vect_Frame.push_back(frame);

        if(supAtime)
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
