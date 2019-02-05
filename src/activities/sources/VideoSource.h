#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H

#include <opencv2/opencv.hpp>
#include <utility>

class VideoSource {

public :

    virtual cv::Mat getColorFeed(){}
    virtual cv::Mat getDepthFeed(){}
    virtual cv::Mat getOriginalDepth(){}
    virtual cv::Mat getMappedFeed(){}
    virtual void update(){}
    virtual bool isRunning() {return true;}
    virtual ~VideoSource() =default;

    virtual bool hasDepthSource() {}
    virtual std::string getTimeStamp(){}
    virtual int getTimePosition(){}
    virtual double getExactTimePosition(){}
    virtual std::pair<int,int> getScreenSize(){}


};


#endif //VIDEOSOURCE_H
