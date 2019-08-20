#include "MatriceAffordance.h"
//TODO Put .h here
/*
 void MatriceAffordance::addnewAff(Affordance* aff, bool removeLastValue)
     {
			this->mat_prec_act.push_back(&aff);
			if(removeLastValue)
			{
				this->mat_prec_act.erase(mat_prec_act.begin());
			}
	}
	*/
/*
static Affordance* MatriceAffordance::updateAffordance(const std::stack<AffordanceTime*> matrice){
	vector<string> nameObject={};
        vector<double> probObject={};
        int j;
        std::stack<AffordanceTime*> allAff= matrice;
        AffordanceTime* searchAff;
        int rang;
        for(int i=0;i<std::min((int)matrice.size(),10);i++){//loop in every elements of the matrix
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
*/
