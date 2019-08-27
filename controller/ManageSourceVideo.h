/**
* \file      ManageSourceVideo.h 
* \author    Mathieu Gravel
* \version   1.0
* \date      13 June 2019
* \brief     Select the good source video including size and handle update 
*/

#pragma once

#include "sources/VideoSource.h"
#include "sources/RealSense.h"
#include "sources/RealSenseVideo.h"
#include "sources/RealSenseBag.h"

#include "sources/Kinect.h"
#include "sources/WebCam.h"

#include <map>
#include <string>
#include <opencv2/opencv.hpp>
#include <list>
#include <cstdlib>

#include <vector>
#include <iostream>
#include <algorithm>


#include "Subject.h"
#include "../model/Observer.h"

using namespace std;
class ManageSourceVideo : public Subject{

public:
	/**
	* \fn ManageSourceVideo()
	* \brief Constructor of class ManageSourceVideo  
	* \param setting : map<string,string>*  which contain every path and information to use (fisrt String = name_of_information, second String = information)
	*/
     ManageSourceVideo(map<string,string> setting);

	/**
	* \fn ~ManageSourceVideo()
	* \brief Destructor of class ManageSourceVideo  
	*/
     virtual ~ManageSourceVideo();
     std::string getTimeStamp() {return timestamp;}
     int getTimePosition() {return timeposition;}

	/**
	* \fn update()
	* \brief 
	*/
     void update();
     
     void chooseInputVideo(string typeInputVideo,int width,int height,string pathSourceVideo,string pathSourceVideoDepth);
     

     cv::Mat getOriginalImage();
     cv::Mat getCurrentImage();
     cv::Mat getDepthImage();
     inline std::pair<int,int> getScreenSize() const {
         return screenSize;
     }
    void saveVideos();

//from observee
	void registerObserver(Observer *observer) override;
//from observee
    void removeObserver(Observer *observer) override;
//from observee
    void notifyObservers() override;
    


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

	std::vector<Observer *> observers; // observers

    bool showPercentage;
    bool showName;
    bool showzone;
    VideoSource* source;
    bool videoSave;
    std::string timestamp;
    int timeposition;
    //AffordanceTime * pastAffordance;


};

