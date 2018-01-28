//
// Created by uqamportable on 28/01/18.
//

#ifndef IMAGEREC_VIDEOSOURCE_H
#define IMAGEREC_VIDEOSOURCE_H

#include <opencv2/opencv.hpp>


class VideoSource {

public :

    virtual cv::Mat getColorFeed(){}
    virtual cv::Mat getDepthFeed(){}
    virtual cv::Mat getMappedFeed(){}
    virtual void update(){}

    virtual ~VideoSource() {}
};


#endif //IMAGEREC_VIDEOSOURCE_H
