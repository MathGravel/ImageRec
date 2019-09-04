#ifndef RealSenseBag_H
#define RealSenseBag_H


#include "VideoSource.h"
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

#define GLFW_INCLUDE_GLU

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include <assert.h>

using namespace rs2;

class RealSenseBag  : public VideoSource{

public:
    RealSenseBag(std::string bagFile = "");

    virtual ~RealSenseBag();


    cv::Mat getColorFeed();
    cv::Mat getDepthFeed();
    cv::Mat getMappedFeed();
    cv::Mat getOriginalDepth(){return depthFeed;}

    void update();
    int getTimePosition() {return (int)((std::clock() - startTime)/(double) CLOCKS_PER_SEC);}
    double getExactTimePosition() {return (std::clock() - startTime)/(double) CLOCKS_PER_SEC;}
    std::pair<int,int> getScreenSize(){return std::pair<int,int>(1280,720);}
    std::string getTimeStamp() {return frame < maxframe ? "-1":"En direct";}


private:
    //A faire, ajouter les composants necessaire pour utiliser la camera realsense.
    cv::Mat depth_frame_to_meters(cv::Mat depthPic);
    cv::Mat frame_to_mat(const rs2::frame& f);
    
	rs2::pipeline_profile pipeline_profile;
    cv::Mat colorFeed;
    cv::Mat depthFeed;
    cv::Mat depthMeters;
    rs2::frameset data;
	std::shared_ptr<rs2::pipeline> pipe;
	double startTime;
    std::string bagVideo;
    bool vidFolder;
    std::string folder;
    int currentDepth;
    int frame, maxframe;

};


#endif //RealSenseBag_H
