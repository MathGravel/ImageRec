//
// Created by uqamportable on 26/04/18.
//
#include "ActivityRegion.h"

ActivityRegion* ActivityRegion::ar_instance = NULL;

ActivityRegion::ActivityRegion():handDetector("/home/uqamportable/CLionProjects/ImageRec/handModel",720,1280,true,0.5f),
                                 objectDetector("/home/uqamportable/CLionProjects/ImageRec/objectModel",720,1280,false,0.4f),
                                 currentlySegmenting(false),newRegions(false),newAffordance(false),oldName("") {
}

 std::vector<cv::Rect> segmentPic(cv::Mat picture,cv::Mat depthPic);

void ActivityRegion::Update(cv::Mat vision,cv::Mat depthVision) {

    std::vector<cv::Rect> objects;
    currentImage = vision;
    currentImageDepth = depthVision;
    currentAffordance.clear();
    hands.clear();
    items.clear();


    if (newRegions) {
        std::vector<DetectedObject> objets;

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

        if (!objets.empty() && !hands.empty()) {

            items = confirmAffordance(objects,vision,depthVision);
        }

    } else {
        items = this->detectObjets(vision, depthVision);
        hands = this->detectHand(vision,depthVision);


        if (!items.empty() && !hands.empty()) {
            currentAffordance = affordances.findAffordances(items, hands);
            if (!currentAffordance.empty()) {

                for (std::vector<AffordanceTime*>::iterator it = currentAffordance.begin();it != currentAffordance.end();++it)
                    currentAffordances.push(*it);

               // oldName = currentAffordance->getAffordance().getName() ;

            }

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
}


cv::Mat ActivityRegion::getImageWithROI() const {
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 1;
    int thickness = 2;
    int baseline = 0;
    cv::Mat pic = currentImage.clone();
    if (! regions.empty()) {
        for (auto &reg : regions) {
            cv::rectangle(pic, reg, cv::Scalar(100, 100, 100),3);
        }
    }
    else if (!items.empty()) {
        for (auto &reg : items) {
            cv::rectangle(pic, reg.getObjPos(), cv::Scalar(0, 0, 250),4);
            if (oldName == reg.getObjName()) {
                cv::rectangle(pic, reg.getObjPos(), cv::Scalar(250, 0, 0),4);

                std::string val = reg.getObjName() + " " + std::to_string((int)floor(reg.getProb() * 100)) + "%";

                cv::Size text = cv::getTextSize(val, fontface, scale, thickness, &baseline);
                cv::Rect textBox(reg.getObjPos());
                textBox.y += textBox.height;
                textBox.height = text.height;
                cv::putText(pic,val,cv::Point(textBox.x,textBox.y + text.height),fontface, scale, CV_RGB(0,250,0), thickness, 8);
            }
      }
    }
    int ii = 0;
    for (auto & hand : hands) {

        cv::rectangle(pic, hand.getObjPos(), cv::Scalar(125, 125, 0), 4);
        std::string val = "Main " + std::to_string((int) floor(hand.getProb() * 100)) + "%";

        cv::Size text = cv::getTextSize(val, fontface, scale, thickness, &baseline);
        cv::Rect textBox(hand.getObjPos());
        textBox.y += textBox.height;
        textBox.height = text.height;
        cv::putText(pic, val, cv::Point(textBox.x, textBox.y + text.height), fontface, scale, CV_RGB(0, 250, 0),
                    thickness, 8);

    }

    if (!currentAffordance.empty()) {
        for (std::vector<AffordanceTime *>::const_iterator it = currentAffordance.begin(); it != currentAffordance.end();it++) {

            Affordance &pos = (*it)->getAffordance();
            cv::rectangle(pic, pos.getRegion(), cv::Scalar(250, 0, 0), 4);

            std::string val = pos.getName() + " " + std::to_string((int) floor(pos.getObjectProbability() * 100)) + "%";

            cv::Size text = cv::getTextSize(val, fontface, scale, thickness, &baseline);
            cv::Rect textBox(pos.getRegion());
            textBox.y += textBox.height;
            textBox.height = text.height;
            cv::putText(pic, val, cv::Point(textBox.x, textBox.y + text.height), fontface, scale, CV_RGB(0, 250, 0),
                        thickness, 8);
        }
    }


    return pic;
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



template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

DetectedObjects ActivityRegion::detectHand(cv::Mat color, cv::Mat depth) {
    return DetectedObjects(handDetector.findObjects(color,depth)) ;
}

DetectedObjects ActivityRegion::detectObjets(cv::Mat color, cv::Mat depth) {
    return DetectedObjects(objectDetector.findObjects(color,depth)) ;
}