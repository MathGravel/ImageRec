//
// Created by uqamportable on 26/04/18.
//

#ifndef IMAGEREC_ACTIVITYREGION_H
#define IMAGEREC_ACTIVITYREGION_H


#include "string"
#include "../Affordance.h"
#include "../ObjectAff.h"
#include "../DetectedObject.h"
#include "../selectiveSearchDepth.h"
#include <opencv2/opencv.hpp>
#include "ImageSegmentationManual.h"
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include "CNN/CaffeCNN.h"
#include "CNN/ImgSegCNN.h"
#include "../MaskRCNN.h"


class ActivityRegion {

public:
    ActivityRegion();

    cv::Mat getChosenRoi() {
        return chosenROI;
    }

    bool hasChosenROI(){
        return chosedROI;
    }

    void setLocalSegmentation() {
        localRec = !localRec;
    }

    void setSegImage() {segImg = !segImg;}
    std::string classe = "";
    bool globalRec = false;
    bool showi = false;
    void SavePictures();
    void detectHand(cv::Mat color, cv::Mat depth,std::vector<cv::Rect>& rect);
    void classifyPic(cv::Mat& currentPic);
    cv::Mat FindRegionProposals(cv::Mat picToSeg);


private:

    void mergeOverlappingBoxes(std::vector<cv::Rect> &inputBoxes, cv::Mat &image, std::vector<cv::Rect> &outputBoxes);

    ObjectAffordances affordances;

    Affordance getCurrentAffordance();


    void Update();

    std::vector<cv::Rect> regions;
    std::vector<std::string> probs;


    std::future<std::vector<cv::Rect>> resultSeg;
    cv::Mat chosenROI;
    bool chosedROI;
    bool localRec;
    bool segImg;

    MaskRCNN handDetector;




};



#endif //IMAGEREC_ACTIVITYREGION_H
