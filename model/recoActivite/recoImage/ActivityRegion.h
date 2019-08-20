/**
 * \file      ActivityRegion.h
 * \author    Mathieu Gravel
 * \version   2.0
 * \date      8 August 2019
 * \brief     interact with YOLO which gives detected objects and hands 
 */


#ifndef ACTIVITYREGION_H
#define ACTIVITYREGION_H


#include <string>
#include <vector>
#include <sstream>
#include "DetectedObject.h"
#include "selectiveSearchDepth.h"
#include <opencv2/opencv.hpp>
#include <mutex>
#include <thread>         
#include <future>         

#include "cnn/YoloGPU.h"
#include "cnn/YoloCPU.h"

#include <stack>
#include <cmath>

#include <map> //setting
using namespace std;

class ActivityRegion {

    static ActivityRegion *ar_instance;

public:
	
    /**
	* \fn ActivityRegion(map<string,string> setting)
    * \brief Constructor of class ActivityRegion
    * \param setting : map<string,string> which contain every path and information to use (fisrt String = name_of_information, second String = information)
    */
    ActivityRegion(map<string,string> setting);

    /**
    * \fn ActivityRegion* instance(map<string,string> setting)
    * \brief check if ActivityRegion object exist, else create it, then return
    * 	it  
    * \details if it's the first step of the loop (MiseAJourImage) or pointer 
    * 	of ActivityRegion as been delete 
    * \param setting : map<string,string> which contain every path and information to use (fisrt String = name_of_information, second String = information)
    * \return ActivityRegion::ar_instance : object of ActivityRegion
    */
    static ActivityRegion* instance(map<string,string> setting){
        if(!ActivityRegion::ar_instance)
            ActivityRegion::ar_instance = new ActivityRegion(setting);
        return ActivityRegion::ar_instance;
    }

    /**
    * \fn void Update(cv::Mat vision, cv::Mat depthVision)
    * \brief send to YOLO image then update vector of Hands and Objects
    * \param vision : matrice which represent color of the picture
    * \param depthVision : matrice which represent the depth in the picture
    */
    void Update(cv::Mat vision, cv::Mat depthVision);
    
    /**
    * \fn DetectedObjects detectHand(cv::Mat color, cv::Mat depth);
    * \brief from matrix of color and depth, found hands
    * \details Use YOLO
    * \param vision : matrice which represent color of the picture
    * \param depthVision : matrice which represent the depth in the picture
    * \return DetectedObjects	every hands that cnn have found
    */
    DetectedObjects detectHand(cv::Mat color, cv::Mat depth);
    
    /**
    * \fn DetectedObjects detectObjets(cv::Mat color, cv::Mat depth);
    * \brief from matrix of color and depth, found objects
    * \details Use YOLO 
    * \param vision : matrix which represent color of the picture
    * \param depthVision : matrix which represent the depth in the picture
    * \return DetectedObjects : every pbjects that cnn have found
    */
    DetectedObjects detectObjets(cv::Mat color, cv::Mat depth);

	/// YOLO I guess
    void UpdateROI(cv::Mat vision, cv::Mat depthVision) {};//A completer
    void updateManualROI(cv::Mat vision, cv::Mat depthVision, cv::Rect chosenROI);       
	//A refaire avec celui de google
    // Affordance testManuallyROI(cv::Mat vision, cv::Rect chosenROI);
    cv::Mat getImageWithROI() const;
    
    /**
     * \fn reset()
     * \brief Reset vector of hands, objects, and regions 
     */
    void reset();

	/// Getter
    DetectedObjects getHands() {
        return hands;
    }

    DetectedObjects getItems() {
        return items;
    }

	/// public variables
	std::mutex mtx;

private:

    std::string oldName;

    std::vector<cv::Rect> regions;

    DetectedObjects hands;
    DetectedObjects items;

    #ifdef USE_GPU
        YoloGPU objectDetector;
    #else
        YoloCPU objectDetector;
    #endif
    
    std::future<std::vector<cv::Rect>> resultSeg;
    
    // YOLO I guess
    cv::Mat chosenROI;
    bool chosedROI;
    cv::Mat imageROI;
        
    bool localRec;
    
    bool segImg;
    bool currentlySegmenting;
    
    bool newRegions;
    
    cv::Mat currentImage;
    cv::Mat currentImageDepth;

	map<string,string> setting;


    //PrimaryWindow have access to private 
    friend class PrimaryWindow;
};
#endif //ACTIVITYREGION_H
