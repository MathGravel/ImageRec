#include "ActivityRegion.h"

ActivityRegion* ActivityRegion::ar_instance = nullptr;

ActivityRegion::ActivityRegion():
                                 objectDetector(0.31f),
                                 currentlySegmenting(false),newRegions(false),newAffordance(false),oldName("") {
       this->objectsMat.instance();
}


std::vector<cv::Rect> segmentPic(cv::Mat picture,cv::Mat depthPic);

void ActivityRegion::Update(cv::Mat vision,cv::Mat depthVision, bool supAtime) {

    std::vector<cv::Rect> objects;
    currentImage = vision;
    currentImageDepth = depthVision;
    currentAffordance.clear();///azerty2


        auto ii = this->detectObjets(vision, depthVision);

        //Test pour le corpus Kitchen ou les mains sont dans le meme systeme.
        std::vector<DetectedObject> mains;
        std::vector<DetectedObject> newit;

        for (const auto it : ii) {
            if (it.getObjName() == "hand" || it.getObjName() == "Hand") {
                mains.push_back(it);
                //exit(-1);
            }
            else {
                newit.push_back(it);
            }

        }
        mtx.lock();
        hands.clear();
        items.clear();
        hands = DetectedObjects(mains);
        items = DetectedObjects(newit);
        mtx.unlock();

        if (!items.empty() && !hands.empty()) {
            currentAffordance = affordances.findAffordances(items, hands, objectsMat,supAtime);///azerty
            if (!currentAffordance.empty()) {///azerty2
                         for (auto it : currentAffordance)
                    currentAffordances.push(it);
            }
        }

}

void ActivityRegion::updateManualROI(cv::Mat vision, cv::Mat depthVision, cv::Rect chosenROI) {

    currentImage = vision;
    currentImageDepth = depthVision;
    hands.clear();
    items.clear();

    regions.push_back(chosenROI);

    hands = this->detectHand(vision,depthVision);

    //A completer.

}
/*
Affordance ActivityRegion::testManuallyROI(cv::Mat vision,  cv::Rect chosenROI) {
    std::pair<std::string,float> prediction = caffe.predict(vision(chosenROI)) ;
    Affordance aff(prediction.first,prediction.second,chosenROI,prediction.second);
    return aff;
}


std::vector<DetectedObject> ActivityRegion::confirmAffordance(const std::vector<cv::Rect>& objets, const cv::Mat & picture, const cv::Mat & depth) {

    std::vector<DetectedObject> classes;

    for (auto & region : objets) {
        for (auto& hand : hands) {
        if ( (region & hand.getObjPos()).area() > 0) {
            std::pair<std::string, float> prediction = caffe.predict(picture(region));
            DetectedObject detected(region, prediction.first, mean(depth(region))[0], prediction.second);
            classes.push_back(detected);
        }
        }
    }
    return classes;
}*/




std::vector<cv::Rect> segmentPic(cv::Mat picture,const cv::Mat depthPic) {

    cv::Mat currentSmall = picture.clone();
    cv::Mat currentDepthSmall = depthPic.clone();

    int height = picture.rows;
    int width = picture.cols;
    int newHeight = height;
    int newWidth = width;
    double ratiox = 1;
    double ratioy = 1;
    while (newHeight > 256 && newWidth > 200) {
        newHeight = newHeight / 1.25;
        ratiox = ratiox * 1.25;
        newWidth = newWidth / 1.25;
    }


    resize(picture, currentSmall, cv::Size(newWidth, newHeight));
    resize(depthPic, currentDepthSmall, cv::Size(newWidth, newHeight));

    auto regions = selectiveDepth::selectiveSearchDepth(currentSmall,currentDepthSmall, 150, 0.9, 30, 300, currentSmall.rows * currentSmall.cols/6, 50);

    std::vector<cv::Rect> newRect;
    cv::groupRectangles(regions,1,0.1);

    // do something...
    std::vector<cv::Rect> resizedRegions;
    for (auto &&rect : regions) {

        cv::Rect x(rect.x * ratiox, rect.y * ratiox, rect.width * ratiox, rect.height * ratiox);
        resizedRegions.push_back(x);
    }

    return resizedRegions;

}



template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

DetectedObjects ActivityRegion::detectHand(cv::Mat color, cv::Mat depth) {
    return DetectedObjects(objectDetector.findObjects(color,depth)) ;
}

DetectedObjects ActivityRegion::detectObjets(cv::Mat color, cv::Mat depth) {
    return DetectedObjects(objectDetector.findObjects(color,depth)) ;
}

void ActivityRegion::reset() {
    currentAffordance.clear();///azerty2

    this->affordances.clearCurrentAffordances();
    this->hands.clear();
    this->items.clear();
    this->regions.clear();
}
