#ifndef RECOMANAGER_H
#define RECOMANAGER_H

#include "sources/VideoFeedTypes.h"
#include "Serializable.h"
#include "ActivityRegion.h"
#include "ImageTreatment.h"
#include "Policy.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>

class RecoManager : public Serializable
{
    public:
        RecoManager(std::map<std::string,std::string> stream);
        virtual ~RecoManager();
        void update();
        void deserialize(std::map<std::string,std::string> stream);
        std::string getTimeStamp() {return feedSource.getTimeStamp();}
        int getTimePosition() {return feedSource.getTimePosition();}
        cv::Mat getOriginalPicture() { return feedSource.getOriginalImage();}
        cv::Mat getCurrentFeed() { return colorPic;}
        cv::Mat getCurrentDepthFeed() { return depthPic;}

    private:
        ActivityRegion* act;
        ImageTreatment feedSource;
        Policy pol;
        cv::Mat colorPic;
        cv::Mat depthPic;
        std::mutex mtx;
        std::atomic<bool> isStopped;
        std::thread * th;
        void updatePolicy();
        void start_thread();
        void start_affordance_check();
};

#endif //RECOMANAGER_H
