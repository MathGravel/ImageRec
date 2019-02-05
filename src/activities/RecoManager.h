#ifndef RECOMANAGER_H
#define RECOMANAGER_H

#include "sources/VideoFeedTypes.h"
#include "Serializable.h"
#include "ActivityRegion.h"
#include "ImageTreatment.h"
#include "Tracemanager.h"
#include "Tracedistances.h"

#include "../plans/include/Domain.h"
#include "../plans/include/Solver.h"

#include "Policy.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include <string>
#include <unordered_map>

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
        void setIsStopped(bool play) {isStopped = play;}
        void start_thread();
        std::map<std::string, std::map<std::string, std::string>> getInformations() { return informations;}
        void reset();
        void saveVideos();
        TraceManager* trace;

    private:
        ActivityRegion* act;
        ImageTreatment feedSource;
#ifdef USE_KITCHEN_DIST
        std::unordered_map<std::string,cv::Rect> objects;
        std::vector<std::string> names;
        TraceDistances * traceD;
#endif

        //Policy pol;
        cv::Mat colorPic;
        cv::Mat depthPic;
        std::mutex mtx;
        bool isStopped = false;
        std::atomic_bool check;

        std::thread * th;
        void updatePolicy();
        void start_affordance_check();
        std::map<std::string, std::map<std::string, std::string>> informations;
};

#endif //RECOMANAGER_H
