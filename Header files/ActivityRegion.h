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


class ActivityRegion {

public:
    ActivityRegion();

    cv::Mat getChosenRoi() {
        return chosenROI;
    }

    bool hasChosenROI(){
        return chosedROI;
    }

    bool isLocalSegmentation() {
        return localRec;
    }

    void setLocalSegmentation() {
        localRec = !localRec;
    }

    std::stack<AffordanceTime*> currentAffordances;

    cv::Rect getHandRegion() {
        return hand;
    }

    const std::vector<cv::Rect>& getRegions() const {
        return regions;
    }


    cv::Mat getOriginalImage() {
        return currentImage;
    }

    cv::Mat getOriginalImageDepth() {
        return currentImageDepth;
    }

    cv::Mat getImageWithROI() {

        cv::Mat pic = currentImage.clone();
        if (! regions.empty()) {
            for (auto &reg : regions) {
                cv::rectangle(pic, reg, cv::Scalar(100, 100, 100));
            }
        }
        else if ()
        if (hand.x != 0 && hand.y != 0)
            cv::rectangle(pic,hand,cv::Scalar(100,50,25));

        return pic;
    }

    cv::Mat getImageWithROIText() {

        cv::Mat pic = currentImage.clone();
        for (auto& reg : affordances.getAffordances()) {
            cv::rectangle(pic,reg.getAffordance().getRegion(),cv::Scalar(100,100,100));
            Affordance aff = reg.getAffordance();
            cv::Point p(aff.getRegion().x,aff.getRegion().y);
            cv::putText(pic,aff.getName() + " " + std::to_string(aff.getObjectProbability()),p,4,2,cv::Scalar(200,30,35));
        }
        cv::rectangle(pic,hand,cv::Scalar(100,50,25));

        return pic;
    }


    std::string getAffordanceString();

    Affordance getCurrentAffordance();


    bool AffordanceUpdated() const {
        return newAffordance;
    };

    std::vector<cv::Rect> getObjectsROI() {
        return regions;
    }


    void setSegImage() {segImg = !segImg;}


    std::vector<DetectedObject> detectHand(cv::Mat color, cv::Mat depth);
    std::vector<DetectedObject> detectObjets(cv::Mat color, cv::Mat depth);

        void classifyPic(cv::Mat& currentPic);
    cv::Mat FindRegionProposals(cv::Mat picToSeg);



    void Update(cv::Mat vision, cv::Mat depthVision);
    void updateManualROI(cv::Mat vision, cv::Mat depthVision, cv::Rect chosenROI);
    Affordance testManuallyROI(cv::Mat vision, cv::Rect chosenROI);


private:

    void mergeOverlappingBoxes(std::vector<cv::Rect> &inputBoxes, cv::Mat &image, std::vector<cv::Rect> &outputBoxes);
    std::vector<DetectedObject> confirmAffordance(const std::vector<cv::Rect>& objets,
                                                   const DetectedObject& hand,const cv::Mat& picture, const cv::Mat& depth);

    std::string classe = "";
    bool globalRec = false;
    bool showi = false;

    ObjectAffordances affordances;

    AffordanceTime* currentAffordance;



    std::vector<cv::Rect> regions;
    cv::Rect hand;

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
