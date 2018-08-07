#ifndef ACTIVITYREGION_H
#define ACTIVITYREGION_H


#include <string>
#include <vector>
#include <sstream>
#include "Affordance.h"
#include "ObjectAff.h"
#include "DetectedObject.h"
#include "selectiveSearchDepth.h"
#include "Serializable.h"
#include <opencv2/opencv.hpp>

#include <thread>         // std::thread
#include <future>         // std::promise, std::future
//#include "cnn/CaffeCNN.h"
#include "cnn/ImgSegCNN.h"
#include "cnn/MaskRCNN.h"
#include "cnn/YoloGPU.h"
#include <stack>
#include <cmath>

class ActivityRegion : public Serializable  {

    static ActivityRegion *ar_instance;

public:
    ActivityRegion();

    static ActivityRegion* instance(){
        if(!ActivityRegion::ar_instance)
            ActivityRegion::ar_instance = new ActivityRegion();
        return ActivityRegion::ar_instance;
    }

    void deserialize(std::map<std::string,std::string> stream){}

    std::stack<AffordanceTime*> currentAffordances;

    cv::Mat getImageWithROI() const;


    bool AffordanceUpdated() const {
        return newAffordance;
    };



    DetectedObjects detectHand(cv::Mat color, cv::Mat depth);
    DetectedObjects detectObjets(cv::Mat color, cv::Mat depth);


    void Update(cv::Mat vision, cv::Mat depthVision);
    void UpdateROI(cv::Mat vision, cv::Mat depthVision) {};//A completer


    void updateManualROI(cv::Mat vision, cv::Mat depthVision, cv::Rect chosenROI);
   //A refaire avec celui de google
    // Affordance testManuallyROI(cv::Mat vision, cv::Rect chosenROI);


private:
    /*
    std::vector<DetectedObject> confirmAffordance(const std::vector<cv::Rect>& objets, const cv::Mat& picture, const cv::Mat& depth);
    */


    ObjectAffordances affordances;

    std::vector<AffordanceTime*> currentAffordance;
    std::string oldName;



    std::vector<cv::Rect> regions;

    DetectedObjects hands;
    DetectedObjects items;

    //ImgSegCNN caffe;
    MaskRCNN handDetector;
    MaskRCNN objectDetector;

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





};
#endif //ACTIVITYREGION_H
