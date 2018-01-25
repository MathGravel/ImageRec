//
// Created by uqamportable on 24/01/18.
//

#ifndef IMAGEREC_IMAGESEGMENTATIONMANUAL_H
#define IMAGEREC_IMAGESEGMENTATIONMANUAL_H


#include <opencv2/opencv.hpp>
#include <vector>
#include <map>
#include <list>

#define MAXTHRESHOLD 180
#define MINTHRESHOLD 120
#define USECOLORTHREASH 1
#define USEHISTEQUAL 0

using namespace std;
using namespace cv;


class ImageSegmentationManual {

public:

    enum ImageFilters { BLUR, MEDIAN,BILFILTER, HISTEQUAL, CANNY,THRESHOLD };

    ImageSegmentationManual();

    virtual ~ImageSegmentationManual();


    cv::Mat segmentPhoto(cv::Mat picture);
    void setThreshhold(int threshMin, int threshMax);



private:
    cv::Mat normalizePicHistogram(cv::Mat picture) const;
    cv::Mat  segmentPic(cv::Mat picture) ;
    cv::Mat threshHoldColors(cv::Mat imgInput) const;
    int minThresh = MINTHRESHOLD;
    int maxThresh = MAXTHRESHOLD;
    std::map<ImageFilters, bool> filterUsed;
    std::list<cv::Mat> pastPictures;
    std::vector<cv::Mat> currentShapes;




};


#endif //IMAGEREC_IMAGESEGMENTATIONMANUAL_H
