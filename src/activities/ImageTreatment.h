#ifndef IMAGETREATMENT_H
#define IMAGETREATMENT_H

#include "Serializable.h"
#include "sources/VideoSource.h"
#include "sources/RealSense.h"
#include "sources/RealSenseVideo.h"
#include "sources/Kinect.h"
#include "sources/WebCam.h"
#include "ActivityRegion.h"

#include <map>
#include <string>
#include <opencv2/opencv.hpp>
#include <list>
#include <cstdlib>


class ImageTreatment : public Serializable {

public:
     ImageTreatment();

     ~ImageTreatment();
     void deserialize(std::map<std::string,std::string> stream);
     std::string getTimeStamp() {return timestamp;}
     int getTimePosition() {return timeposition;}

     void update();
     void treatPicture(ActivityRegion* act);

     cv::Mat getOriginalImage();
     cv::Mat getCurrentImage();
     cv::Mat getDepthImage();


private:

    void saveVideos();

    cv::Mat colorPicture;
    cv::Mat depthPicture;
    cv::Mat imageTreated;
    cv::Mat resizedPicture;
    //cv::VideoWriter colorVid;
    //cv::VideoWriter depthVid;
    std::list<cv::Mat> colorFeed;
    std::list<cv::Mat> programFeed;
    std::list<cv::Mat> depthFeed;
    std::pair<int,int> screenSize;
    std::pair<int,int> originalSize;
    bool showPercentage;
    bool showName;
    bool showzone;
    VideoSource* source;
    bool videoSave;
    std::string timestamp;
    int timeposition;

};


#endif //IMAGETREATMENT_H
