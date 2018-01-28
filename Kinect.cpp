//
// Created by uqamportable on 19/01/18.
//

#include "Kinect.h"

Kinect::Kinect()

{
    std::cout << "Streaming from Kinect One sensor!" << std::endl;
    this->acquire();
    //! [context]

    //! [context]

    //! [discovery]
    if(freenect2.enumerateDevices() == 0)
    {
        std::cout << "no device connected!" << std::endl;
        return ;
    }

    std::string serial = freenect2.getDefaultDeviceSerialNumber();

    std::cout << "SERIAL: " << serial << std::endl;


    if(pipeline)
    {
        //! [open]
        dev = freenect2.openDevice(serial, pipeline);
        //! [open]
    } else {
        dev = freenect2.openDevice(serial);
    }

    if(dev == 0)
    {
        std::cout << "failure opening device!" << std::endl;
        return ;
    }



    //! [listeners]
    listener = new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color |
                                                        libfreenect2::Frame::Depth |
                                                        libfreenect2::Frame::Ir);

    dev->setColorFrameListener(listener);
    dev->setIrAndDepthFrameListener(listener);
    //! [listeners]

    //! [start]
    dev->start();

    std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
    std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;
    //! [start]

    //! [registration setup]
    registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
    //! [registration setup]

    this->release();



}


Kinect::~Kinect() {
    delete registration;
    dev->stop();
    dev->close();
}


cv::Mat Kinect::getColorFeed(){
    return rgbmat;
}

cv::Mat Kinect::getDepthFeed(){
    return depthmat;
}
cv::Mat Kinect::getMappedFeed(){
    return rgbd2;
}

void Kinect::update() {

    this->acquire();
    libfreenect2::FrameMap frames;
    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4); // check here (https://github.com/OpenKinect/libfreenect2/issues/337) and here (https://github.com/OpenKinect/libfreenect2/issues/464) why depth2rgb image should be bigger

    listener->waitForNewFrame(frames);
    libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
    libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
    libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];
    //! [loop start]

    cv::Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);
    cv::Mat(ir->height, ir->width, CV_32FC1, ir->data).copyTo(irmat);
    cv::Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthmat);



    //! [registration]
    registration->apply(rgb, depth, &undistorted, &registered, true, &depth2rgb);
    //! [registration]

    cv::Mat(undistorted.height, undistorted.width, CV_32FC1, undistorted.data).copyTo(depthmatUndistorted);
    cv::Mat(registered.height, registered.width, CV_8UC4, registered.data).copyTo(rgbd);
    cv::Mat(depth2rgb.height, depth2rgb.width, CV_32FC1, depth2rgb.data).copyTo(rgbd2);

    listener->release(frames);
    this->release();

}
