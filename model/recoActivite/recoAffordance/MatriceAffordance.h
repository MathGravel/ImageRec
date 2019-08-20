/**
* \file MatriceAffordance.h
* \author 
* \version 2.0
* \date 14/08/2019
* \brief keep in memory the last affordances seen (frame in the last 800ms : in version 2.0) PUT in cpp 
**/

#ifndef OBJECTSMAT_H
#define OBJECTSMAT_H

#pragma once

#include <iostream> 

#include <vector>
#include <unordered_map>
#include <stack>

#include <cmath>

#include "Affordance.h"

using namespace std;

class MatriceAffordance {
	
    const string NULLNAME = "NULL"; // Name when their is no affordance seen
    const double PROBABILITY_WHEN_VECTOR_EMPTY = 0.5; //probability for the first affordance seen
    
public:

    /**
    * \fn	MatriceAffordance()
    * \brief	Constructor of MatriceAffordance
    * \details initializes mat_objets, matrix of objects
    **/
    MatriceAffordance() {
        instance();
    }

    /**
     * \fn reset()
     * \brief removes all the elements from mat_objects
     */
    void reset(){
        this->mat_objets.clear();
    }

    /**
     * \fn instance()
     */
    void instance(){
        mat_objets = {};
    }

    /**
     * \fn updateAffordance(const std::stack<AffordanceTime*> matrice)
     * \brief from the stack of AffordanceTime* return the most likely affordance of the last frame
     * \details with vector of each affordance order by probability for each frame
     * \param const std::stack<AffordanceTime*> matrice : vector/stack of vector of affordanceTime (first : each frame, second : each possibility for this frame
     */
    static Affordance* updateAffordance(const std::stack<AffordanceTime*> matrice){
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
        // We already check that currentAffordances != empty

        int rangMax=0;
        for(int a=1;a<probObject.size();a++){
            if(probObject[rangMax]<probObject[a]){
                rangMax=a;
            }
        }        
        return new Affordance(nameObject[rangMax],0,cv::Rect(),probObject[rangMax]/std::min((int)matrice.size(),10),0);
}

	/**
	 * \fn addnewAff(Affordance aff, bool removeLastValue)
     * \brief update vector of last aff
     * \param Affordance aff : last affordance seen
     * \param bool removeLastValue : indicates if the time since the start is great enough for start to delete the oldest vector of affordance
     */
	void addnewAff(Affordance* aff,bool removeLastValue){
		this->mat_prec_act.push_back(aff);
		if(removeLastValue){
			this->mat_prec_act.erase(mat_prec_act.begin());
		}
	}
    
	
    /**
     * \fn update(std::vector<AffordanceTime*> objs, bool removeLastValue)
     * \brief add last AffordanceTime(vector of Affordance = every affordance possible for one frame) and suppress the oldest one if time since the start is great enough
     * \details 
     * \param std::vector<AffordanceTime*> objs : AffordanceTime(vector of Affordance = every affordance possible for one frame)
     * \param bool removeLastValue : indicates if the time since the start is great enough for start to delete the oldest vector of affordance
     */
    void update(std::vector<AffordanceTime*> objs, bool removeLastValue){
        mat_objets.push_back(objs);
        if(removeLastValue)
        {
			//cout<<"lastValue erased"<<endl;
            this->mat_objets.erase(mat_objets.begin());
        }
    }


	//Never Used
    /**
     * \fn getFrequence(String name)
     * \brief returns the frequence of the affordance with an object in the last images | Never Used
     * \param name : Name of the  class of the object to check
     * \return the frequence of the object in the last images and itself
     */
    double getFrequence(string name)
    {   
		bool trouve=false;
        double nb=1; //Because this one is part of "mat_objets" but didn't added yet
        for(auto listAff : mat_objets){	 //vector of affordanceTime (a vector of Affordance = every affordance possible for one frame)
			trouve=false;
            for(auto item : listAff){
				if(item->getAffordance().getName()==name){
					
					trouve = true;//If we found one in vector of a frame couln't appear a second time
					nb++;
					//nb=nb+0.6+(item->getAffordance().getObjectProbability())*0.4;
				}
			}
        }
        //Return number of time seen in last affordances divide by the size if  != 0 else return a const value
        if (mat_objets.size()>0)
            return (double)nb/(double)(mat_objets.size()+1);
        else return PROBABILITY_WHEN_VECTOR_EMPTY;
    }
   
	/**
	 * \fn probCalculation(string name,double prob_object)
	 * \brief Calculation of the average probability : weighted average
	 * \details (prob_of_object + sum(for i=2 to n) of (i/n) * prob_same_object_affordance_at_i_frame_of_vector )/ sum(for i=1 to n)  of i/n
	 * \param name : Name of the  class of the object to check
	 * \param prob_object : probabilty of image recognition of the object (probability written on the screen in treadPicture
	 * \return double : probabilty of the affordance
	 */
    double probCalculation(string name,double prob_object){
		double divisorSum=0; //sum(for i=1 to n)  of i/n
		int n=mat_objets.size()+1;
		for(int i=1;i<=n;i++){
			divisorSum+=i/n;
		}


		double numerator;//numerator= prob_of_object + sum(for i=2 to n) of (i/n) * prob_same_object_affordance_at_i_frame_of_vector
		
		numerator=prob_object;//numerator= prob_of_object
		
		
		for(int i=0;i<n-1;i++){	 //vector of affordanceTime (a vector of Affordance = every affordance possible for one frame

			auto listAff=mat_objets[i];
            for(auto item : listAff){ 

				if(item->getAffordance().getName()==name){
					numerator+=(i/n)*item->getAffordance().getObjectProbability(); // numerator += (i/n) * prob_same_object_affordance_at_i_frame_of_vector
					break;				//if found one, can't found an other one
				}
			}
        }
        return (numerator/divisorSum);
   }     
   bool moreThanAffFrame(string nom_aff,int that){
			int countFrame=0;
			for(auto aff : this->mat_prec_act){
				if(aff->getName()==nom_aff)
					countFrame++;
			}
		return countFrame>that;
	}
	
    

    /**
     * \fn get_affordances()
     */
    std::vector<std::vector<AffordanceTime*>> get_affordances() const { return mat_objets;}
    std::vector<Affordance*> get_Mat_prec_act ()const { return mat_prec_act;}
private:

    std::vector<std::vector<AffordanceTime*>> mat_objets;
    std::vector<Affordance*> mat_prec_act;



};
#endif //OBJECTSMAT_H
