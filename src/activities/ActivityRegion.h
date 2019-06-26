/**
 * \file      ActivityRegion.h
 * \author    Mathieu Gravel
 * \version   1.0
 * \date      13 June 2019
 * \brief    
 * \details
 */


#ifndef ACTIVITYREGION_H
#define ACTIVITYREGION_H


#include <string>
#include <vector>
#include <sstream>
#include "Affordance.h"
#include "ObjectAff.h"
#include "ObjectsMat.h"
#include "DetectedObject.h"
#include "selectiveSearchDepth.h"
#include "Serializable.h"
#include <opencv2/opencv.hpp>
#include <mutex>
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
//#include "cnn/CaffeCNN.h"
#include "cnn/YoloGPU.h"
#include "cnn/YoloCPU.h"

#include <stack>
#include <cmath>

class ActivityRegion : public Serializable  {

    static ActivityRegion *ar_instance;

public:
	
    /**
	* \fn ActivityRegion()
    * \brief Constructor of class ActivityRegion  
    * \details  Initialize composant of ActivityRegion
    */
    ActivityRegion();

    /**
    * \fn ActivityRegion* instance()
    * \brief check if ActivityRegion object exist, else create it, then return
    * 	it  
    * \details if it's the first step of the loop (MiseAJourImage) or pointer 
    * 	of ActivityRegion as been delete 
    * \return ActivityRegion::ar_instance	object of ActivityRegion
    */
    static ActivityRegion* instance(){
        if(!ActivityRegion::ar_instance)
            ActivityRegion::ar_instance = new ActivityRegion();
        return ActivityRegion::ar_instance;
    }

    void deserialize(std::map<std::string,std::string> stream){}

    std::stack<AffordanceTime*> currentAffordances;

    cv::Mat getImageWithROI() const;


    bool AffordanceUpdated() const {
        return affordances.currentlyHasAffordances();
    };

    DetectedObjects detectHand(cv::Mat color, cv::Mat depth);
    DetectedObjects detectObjets(cv::Mat color, cv::Mat depth);
    std::mutex mtx;

    /**
    * \fn void Update(cv::Mat vision, cv::Mat depthVision)
    * \brief 
    * \details 
    * \param vision : matrice which represent color of the picture
    * \param depthVision : matrice which represent the depth in the picture
    */
    void Update(cv::Mat vision, cv::Mat depthVision);
    void UpdateROI(cv::Mat vision, cv::Mat depthVision) {};//A completer


    void updateManualROI(cv::Mat vision, cv::Mat depthVision, cv::Rect chosenROI);
    void reset();

   /* #ifdef USE_KITCHEN_DIST
    DetectedObjects hands;
    DetectedObjects items;
    #endif*/
    DetectedObjects getHands() {
        return hands;
    }

    DetectedObjects getItems() {
        return items;
    }



   //A refaire avec celui de google
    // Affordance testManuallyROI(cv::Mat vision, cv::Rect chosenROI);


private:
    /*
    std::vector<DetectedObject> confirmAffordance(const std::vector<cv::Rect>& objets, const cv::Mat& picture, const cv::Mat& depth);
    */


    ObjectAffordances affordances;

    std::vector<AffordanceTime*> currentAffordance;///azerty2

    std::string oldName;



    std::vector<cv::Rect> regions;


   // #ifndef USE_KITCHEN_DIST
        DetectedObjects hands;
        DetectedObjects items;
   // #endif
    //ImgSegCNN caffe;
    //MaskRCNN handDetector;
    #ifdef USE_GPU
        YoloGPU objectDetector;
    #else
        YoloCPU objectDetector;
    #endif
    std::future<std::vector<cv::Rect>> resultSeg;
    cv::Mat chosenROI;
    bool chosedROI;
    bool localRec;
    bool segImg;
    bool currentlySegmenting;
    bool newRegions;
    bool newAffordance;
    cv::Mat currentImage;
    cv::Mat currentImageDepth;
    cv::Mat imageROI;
    friend class ImageTreatment;
    ObjectsMat objectsMat;



};
#endif //ACTIVITYREGION_H
