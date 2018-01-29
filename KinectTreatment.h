//
// Created by uqamportable on 28/01/18.
//

#ifndef IMAGEREC_KINECTTREATMENT_H
#define IMAGEREC_KINECTTREATMENT_H

#include "ImageSegmentationManual.h"
#include "VideoSource.h"


class KinectTreatment : public VideoSource {

public:
    KinectTreatment(VideoSource * feed);


     cv::Mat getColorFeed();
     cv::Mat getDepthFeed();
     cv::Mat getMappedFeed();
     void update();

     ~KinectTreatment() {}
private:
    VideoSource * vidFeed;
    ImageSegmentationManual segImage;





};


#endif //IMAGEREC_KINECTTREATMENT_H
