//
// Created by uqamportable on 26/04/18.
//

#ifndef IMAGEREC_ACTIVITYREGION_H
#define IMAGEREC_ACTIVITYREGION_H


#include <string>
#include <vector>
#include <sstream>
#include "../Affordance.h"
#include "../ObjectAff.h"
#include "../DetectedObject.h"
#include "../selectiveSearchDepth.h"
#include <opencv2/opencv.hpp>
#include "ImageSegmentationManual.h"
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
//#include "CNN/CaffeCNN.h"
#include "CNN/ImgSegCNN.h"
#include "../MaskRCNN.h"
#include "../Yolo.h"

#include <stack>
#include <math.h>

class ActivityRegion {

    static ActivityRegion *ar_instance;

public:
    ActivityRegion();

    static ActivityRegion* instance(){
        if(!ActivityRegion::ar_instance)
            ActivityRegion::ar_instance = new ActivityRegion();
        return ActivityRegion::ar_instance;
    }

    std::stack<AffordanceTime*> currentAffordances;

    cv::Mat getImageWithROI() const;


    bool AffordanceUpdated() const {
        return newAffordance;
    };



    DetectedObjects detectHand(cv::Mat color, cv::Mat depth);
    DetectedObjects detectObjets(cv::Mat color, cv::Mat depth);


    void Update(cv::Mat vision, cv::Mat depthVision);
    void updateManualROI(cv::Mat vision, cv::Mat depthVision, cv::Rect chosenROI);
    Affordance testManuallyROI(cv::Mat vision, cv::Rect chosenROI);


private:

    void mergeOverlappingBoxes(std::vector<cv::Rect> &inputBoxes, cv::Mat &image, std::vector<cv::Rect> &outputBoxes);
    std::vector<DetectedObject> confirmAffordance(const std::vector<cv::Rect>& objets, const cv::Mat& picture, const cv::Mat& depth);



    ObjectAffordances affordances;

    std::vector<AffordanceTime*> currentAffordance;
    std::string oldName;



    std::vector<cv::Rect> regions;

    DetectedObjects hands;
    DetectedObjects items;

    ImgSegCNN caffe;
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






};



#endif //IMAGEREC_ACTIVITYREGION_H
