//
// Created by uqamportable on 26/04/18.
//
#include "ActivityRegion.h"


ActivityRegion::ActivityRegion() {

}


Affordance ActivityRegion::getCurrentAffordance() {
    return affordances.getAffordances().front().getAffordance();
}

void ActivityRegion::Update() {

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

void ActivityRegion::detectHand(cv::Mat color, cv::Mat depth,std::vector<cv::Rect>& rect) {

    for (auto& handPos : handDetector.findObjects(color,depth)) {
        rect.push_back(handPos.getObjPos());
    }

}