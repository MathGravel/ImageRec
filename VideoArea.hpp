/* 
 * File:   VideoArea.hpp
 * Author: imam
 *
 * Created on 3 October 2015, 1:03 AM
 */

#ifndef VIDEOAREA_HPP
#define	VIDEOAREA_HPP

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include "selectiveSearchDepth.h"
#include <gdkmm/general.h>
#include <glibmm/main.h>
#include <opencv2/opencv.hpp>
#include "Kinect.h"
#include "ImageSegmentationManual.h"
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include "Header files/CNN/CaffeCNN.h"
#include "Header files/CNN/ImgSegCNN.h"

#include "Socket.h"
#include "MaskRCNN.h"
#include "DetectedObject.h"
#include "Header files/ActivityRegion.h"


class VideoArea : public Gtk::DrawingArea
{

public:
    VideoArea ();
    void StartCamera(VideoSource * feed = NULL);
    void RestartCamera();
    void SegmentClassifyROI();

    void StopCamera();
    virtual ~VideoArea();

    cv::Mat getChosenRoi() {
        return chosenROI;
    }

    bool hasChosenROI(){
        return chosedROI;
    }

    void setLocalSegmentation() {
        localRec = !localRec;
    }


    bool hasNewAffordance() {
        return ActivityRegion::instance()->AffordanceUpdated();
    }

    const Affordance GetCurrentAffordance()  {
        AffordanceTime* aff =  ActivityRegion::instance()->currentAffordances.top();
        ActivityRegion::instance()->currentAffordances.pop();
        return aff->getAffordance();
    }


    void setSegImage() {segImg = !segImg;}
    void  SaveROI(const std::string fileLoc, const std::string itemClass);
    std::string classe = "";
    bool globalRec = false;
    bool showi = false;
    void SavePictures();

protected:
    bool cv_opened;
    VideoSource * sourceFeed;


    virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &cr);
    bool onMouseDown(GdkEventButton * event);
    bool onMouseMove(GdkEventMotion * event);
    bool onMouseUp(GdkEventButton * event);
    bool on_timeout ();
    bool segImg = false;
    ImageSegmentationManual imgSeg;

private:
    bool on_dragged;
    bool chosedROI;
    bool localRec;
    bool segmenting = false;
    int skipframes = 0;
    int x1,x2,y1,y2;
    cv::Rect rectROI;
    //CaffeCNN caffe;

    //ImgSegCNN caffe;
    cv::Mat chosenROI;
    cv::Mat currentPic;
    cv::Mat formattedPic;
    cv::Mat currentDepthPic;
    cv::Mat currentMappedPic;
    cv::Mat AddData();
    //MaskRCNN handDetector;
    ActivityRegion* act;
    cv::VideoWriter vidCap;


    std::vector<cv::Rect> regions;
    std::vector<std::string> probs;


    std::future<std::vector<cv::Rect>> resultSeg;


};

#endif	/* VIDEOAREA_HPP */

