#ifndef KINECT_H
#define KINECT_H
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <time.h>
#include <string>

#include <signal.h>
#include <opencv2/opencv.hpp>

#ifdef USE_KINECT
/// [headers]
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#endif
#include "VideoSource.h"

typedef void* HANDLE;
typedef bool BOOLEAN;


class Kinect : public VideoSource {
public:
    Kinect();
    ~Kinect();


    cv::Mat getColorFeed();
    cv::Mat getDepthFeed();
    cv::Mat getMappedFeed();
    cv::Mat getOriginalDepth(){return cv::Mat();}

    void update();
    bool hasDepthSource() {return true;}
    std::string getTimeStamp() {return "<font color=\"#CD2034\">&#149;</font> En direct";}
    int getTimePosition() {return 100;}
    double getExactTimePosition() {return 0;}
    std::pair<int,int> getScreenSize(){return std::pair<int,int>(1280,720);}



private:

#ifdef USE_KINECT

    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device *dev = 0;
    libfreenect2::PacketPipeline *pipeline = 0;
    bool kinectRun = true;
    bool correctlyInit = false;
    libfreenect2::Registration* registration;
    libfreenect2::SyncMultiFrameListener* listener;
    cv::Mat rgbmat, depthmat, depthmatUndistorted, irmat, rgbd, rgbd2;
    //libfreenect2::Frame undistorted,registered,depth2rgb;
#endif


    // ******* multi thread ********
 private:
    pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER;
    BOOLEAN thread_initialized = false;
  void initThread() {
    if (thread_initialized) return;
    pthread_mutex_init(&mutex, NULL);
    thread_initialized = true;
  }
 public:
  void acquire() {
    if (!thread_initialized) initThread();
    pthread_mutex_lock(&mutex);
  }
  void release() {
    if (!thread_initialized) return;
      pthread_mutex_unlock(&mutex);
  }

};



#endif //KINECT_H
