//
// Created by uqamportable on 26/04/18.
//
#include "ActivityRegion.h"


ActivityRegion::ActivityRegion():handDetector("/home/uqamportable/CLionProjects/ImageRec/cnnModel",720,1280),
                                 objectDetector("/home/uqamportable/CLionProjects/ImageRec/objectModel",720,1280),
                                 currentlySegmenting(false),newRegions(false),newAffordance(false) {
}

Affordance ActivityRegion::getCurrentAffordance() {
    return currentAffordance->getAffordance();
}

std::string ActivityRegion::getAffordanceString() {
    std::ostringstream oss;
    oss << currentAffordance;
    return oss.str();
}


 std::vector<cv::Rect> segmentPic(cv::Mat picture,cv::Mat depthPic);

void ActivityRegion::Update(cv::Mat vision,cv::Mat depthVision) {

    std::vector<cv::Rect> objects;
    DetectedObjects objs;
    detect.clear();
    std::vector<DetectedObject> objets;
    std::vector<DetectedObject> hands;
    currentImage = vision;
    currentImageDepth = depthVision;

    if (newRegions) {
        if (!currentlySegmenting) {
            resultSeg = std::async(std::launch::async, segmentPic, vision.clone(),depthVision.clone());
            currentlySegmenting = true;
        } else {
            if (resultSeg.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                regions.clear();
                objects = resultSeg.get();
                regions = objects;
                currentlySegmenting = false;
            }
        }
        hands = this->detectHand(vision,depthVision);
        if (!hands.empty())
            hand = cv::Rect(hands.front().getObjPos());
        else
            hand = cv::Rect(0,0,0,0);

        if (!objets.empty() && !hands.empty()) {

            detect = confirmAffordance(objects,hands.front(),vision,depthVision);
            objs = DetectedObjects(detect);
        }

    } else {
        detect = this->detectObjets(vision, depthVision);
        hands = this->detectHand(vision,depthVision);
        if (!hands.empty())
            hand = cv::Rect(hands.front().getObjPos());
        else
            hand = cv::Rect(0,0,0,0);
        if (!detect.empty() && hands.empty())
            currentAffordance = affordances.findAffordances(detect, hands.front());
            if (currentAffordance != NULL) {
                currentAffordances.push(currentAffordance);
            }

    }

}

void ActivityRegion::updateManualROI(cv::Mat vision, cv::Mat depthVision, cv::Rect chosenROI) {
    std::vector<cv::Rect> objects;
    DetectedObjects objs;
    std::vector<DetectedObject> detect;
    std::vector<DetectedObject> hands;
    currentImage = vision;
    currentImageDepth = depthVision;


    regions.push_back(chosenROI);

    hands = this->detectHand(vision,depthVision);
    hand = hands.front().getObjPos();
    if (!objects.empty() && !hands.empty()) {

        detect = confirmAffordance(objects,hands.front(),vision,depthVision);
        objs = DetectedObjects(detect);
    }
    if (!detect.empty())
        currentAffordance = affordances.findAffordances(objs,detect.front());
}

Affordance ActivityRegion::testManuallyROI(cv::Mat vision,  cv::Rect chosenROI) {
    std::pair<std::string,float> prediction = caffe.predict(vision(chosenROI)) ;
    Affordance aff(prediction.first,prediction.second,chosenROI,prediction.second);

    return aff;
}


std::vector<DetectedObject> ActivityRegion::confirmAffordance(const std::vector<cv::Rect>& objets, const DetectedObject& hand, const cv::Mat & picture, const cv::Mat & depth) {

    std::vector<DetectedObject> classes;

    for (auto & region : objets) {
        if ( (region & hand.getObjPos()).area() > 0) {
            std::pair<std::string,float> prediction = caffe.predict(picture(region)) ;
            DetectedObject detected(region,prediction.first,mean(depth(region))[0],prediction.second);
            classes.push_back(detected);
        }
    }
    return classes;
}



std::vector<cv::Rect> segmentPic(cv::Mat picture,cv::Mat depthPic) {

    cv::Mat current = picture;
    cv::Mat currentSmall = current.clone();
    cv::Mat currentDepthSmall = depthPic.clone();

    int height = current.rows;
    int width = current.cols;
    int newHeight = height;
    int newWidth = width;
    double ratiox = 1;
    double ratioy = 1;
    while (newHeight > 256 && newWidth > 200) {
        newHeight = newHeight / 1.25;
        ratiox = ratiox * 1.25;
        newWidth = newWidth / 1.25;
    }


    resize(current, currentSmall, cv::Size(newWidth, newHeight));
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

void ActivityRegion::mergeOverlappingBoxes(std::vector<cv::Rect> &inputBoxes, cv::Mat &image, std::vector<cv::Rect> &outputBoxes)
{
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1); // Mask of original image
    cv::Size scaleFactor(10,10); // To expand rectangles, i.e. increase sensitivity to nearby rectangles. Doesn't have to be (10,10)--can be anything
    for (int i = 0; i < inputBoxes.size(); i++)
    {
        cv::Rect box = inputBoxes.at(i) + scaleFactor;
        cv::rectangle(mask, box, cv::Scalar(255), CV_FILLED); // Draw filled bounding boxes on mask
    }

    std::vector<std::vector<cv::Point>> contours;
    // Find contours in mask
    // If bounding boxes overlap, they will be joined by this function call
    cv::findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    for (int j = 0; j < contours.size(); j++)
    {
        outputBoxes.push_back(cv::boundingRect(contours.at(j)));
    }
}

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

std::vector<DetectedObject> ActivityRegion::detectHand(cv::Mat color, cv::Mat depth) {
    return handDetector.findObjects(color,depth) ;
}

std::vector<DetectedObject> ActivityRegion::detectObjets(cv::Mat color, cv::Mat depth) {
    return objectDetector.findObjects(color,depth) ;
}