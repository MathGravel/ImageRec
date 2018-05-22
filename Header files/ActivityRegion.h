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



    std::vector<DetectedObject> detectHand(cv::Mat color, cv::Mat depth);
    std::vector<DetectedObject> detectObjets(cv::Mat color, cv::Mat depth);


    void Update(cv::Mat vision, cv::Mat depthVision);
    void updateManualROI(cv::Mat vision, cv::Mat depthVision, cv::Rect chosenROI);
    Affordance testManuallyROI(cv::Mat vision, cv::Rect chosenROI);


private:

    void mergeOverlappingBoxes(std::vector<cv::Rect> &inputBoxes, cv::Mat &image, std::vector<cv::Rect> &outputBoxes);
    std::vector<DetectedObject> confirmAffordance(const std::vector<cv::Rect>& objets,
                                                   const DetectedObject& hand,const cv::Mat& picture, const cv::Mat& depth);



    ObjectAffordances affordances;

    AffordanceTime* currentAffordance;
    AffordanceTime* currentAffordanceR;
     std::string oldName;



    std::vector<cv::Rect> regions;
    cv::Rect hand;
    cv::Rect handR;
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
    std::vector<DetectedObject> detect;
    cv::Mat imageROI;






};



#endif //IMAGEREC_ACTIVITYREGION_H
