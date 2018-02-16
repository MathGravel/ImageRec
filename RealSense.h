//
// Created by uqamportable on 16/02/18.
//

#ifndef IMAGEREC_REALSENSE_H
#define IMAGEREC_REALSENSE_H

#include "VideoSource.h"

class RealSense  : public VideoSource{

public:
    RealSense();
    ~RealSense();
    cv::Mat getColorFeed();
    cv::Mat getDepthFeed();
    cv::Mat getMappedFeed();
    void update();
private:
    //A faire, ajouter les composants necessaire pour utiliser la camera realsense.


};

#endif //IMAGEREC_REALSENSE_H
