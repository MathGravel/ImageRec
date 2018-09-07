#ifndef REALSENSEVIDEO_H
#define REALSENSEVIDEO_H


#include "VideoSource.h"
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

#define GLFW_INCLUDE_GLU

#include <string>
#include <sstream>
#include <iostream>


class RealSenseVideo  : public VideoSource{

public:
    RealSenseVideo(std::string colorFile = "",std::string depthFile = "");

    virtual ~RealSenseVideo();


    cv::Mat getColorFeed();
    cv::Mat getDepthFeed();
    cv::Mat getMappedFeed();
    cv::Mat getOriginalDepth(){return depthFeed;}

    void update();
    bool hasDepthSource() {return depthVideo != "";}
    std::string getTimeStamp();
    int getTimePosition() {return ((int) (vid->get(cv::CAP_PROP_POS_FRAMES)*100) / vid->get(cv::CAP_PROP_FRAME_COUNT));}
    double getExactTimePosition() {return (int)(((vid->get(cv::CAP_PROP_POS_FRAMES)*100) / vid->get(cv::CAP_PROP_FRAME_COUNT))/0.01) * 0.01;}
    std::pair<int,int> getScreenSize(){return std::make_pair((int)vid->get(cv::CAP_PROP_FRAME_WIDTH),(int)vid->get(cv::CAP_PROP_FRAME_HEIGHT));}


private:
    //A faire, ajouter les composants necessaire pour utiliser la camera realsense.
    cv::Mat depth_frame_to_meters(cv::Mat depthPic);
    bool LoadMatBinary(const std::string& filename, cv::Mat& output);
    bool readMatBinary(std::ifstream& ifs, cv::Mat& in_mat);

    cv::Mat colorFeed;
    cv::Mat depthFeed;
    cv::Mat depthMeters;

    cv::VideoCapture* vid;
    cv::VideoCapture* vidDepth;
    std::string colorVideo;
    std::string depthVideo;
    bool vidFolder;
    std::string folder;
    int currentDepth;

};


#endif //REALSENSEVIDEO_H
