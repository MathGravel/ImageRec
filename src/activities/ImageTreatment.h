/**
* \file      ImageTreatment.h
* \author    Mathieu Gravel
* \version   1.0
* \date      13 June 2019
* \brief     
* \details  
*/

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
	/**
	* \fn ImageTreatment()
	* \brief Constructor of class ImageTreatment  
	*/
     ImageTreatment();

	/**
	* \fn ~ImageTreatment()
	* \brief Destructor of class ImageTreatment  
	*/
     ~ImageTreatment();
     void deserialize(std::map<std::string,std::string> stream);
     std::string getTimeStamp() {return timestamp;}
     int getTimePosition() {return timeposition;}

	/**
	* \fn update()
	* \brief 
	*/
     void update();
     void treatPicture(ActivityRegion* act);

     cv::Mat getOriginalImage();
     cv::Mat getCurrentImage();
     cv::Mat getDepthImage();
     inline std::pair<int,int> getScreenSize() const {
         return screenSize;
     }
     void saveVideos();



private:

    std::pair<int,int> screenSize;
    std::pair<int,int> originalSize;
    cv::Mat colorPicture;
    cv::Mat depthPicture;
    cv::Mat imageTreated;
    cv::Mat resizedPicture;
    //cv::VideoWriter colorVid;
    //cv::VideoWriter depthVid;
    std::list<cv::Mat> colorFeed;
    std::list<cv::Mat> programFeed;
    std::list<cv::Mat> depthFeed;
/*#ifdef USE_KITCHEN_DIST
        std::unordered_map<std::string,cv::Rect> objects;
        std::vector<std::string> names;
        TraceDistances * traceD;
#endif*/

    bool showPercentage;
    bool showName;
    bool showzone;
    VideoSource* source;
    bool videoSave;
    std::string timestamp;
    int timeposition;
    AffordanceTime * pastAffordance;


};


#endif //IMAGETREATMENT_H
