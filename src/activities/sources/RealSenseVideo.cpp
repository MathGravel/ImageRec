#include "sources/RealSenseVideo.h"


RealSenseVideo::RealSenseVideo(std::string colorFile ,std::string depthFile ) : colorVideo(std::move(colorFile)),depthVideo(std::move(depthFile)) {

    vid = new cv::VideoCapture(colorVideo);
    if (depthVideo.back() != 't' && depthVideo.back() != 'g') {
        vidDepth = new cv::VideoCapture(depthVideo);
        vidFolder = false;
        folder = "";
    } else if (depthVideo.back() != 't') {
        depthVideo = depthVideo.erase(depthVideo.find_last_of('-')+1);
        depthVideo.append("%05d.png");
        vidDepth = new cv::VideoCapture(depthVideo);
        vidFolder = false;
        folder = "";
    }
    else
    {

       folder = depthVideo.substr(depthVideo.find_last_of('/') + 1);
       depthVideo = depthVideo.erase(depthVideo.find_last_of('/'));
        folder.erase(folder.find_last_of("-")+1);
       //vidDepth = new cv::VideoCapture(depthVideo + "/%03d.txt");
       vidFolder = true;
       vidDepth = NULL;
    }
    currentDepth = 0;
}

RealSenseVideo::~RealSenseVideo() {
    if (vid != NULL) {
    vid->release();
    delete vid;
    vid = NULL;
    }
    if (vidDepth != NULL) {
        vidDepth->release();
        delete vidDepth;
        vidDepth = NULL;
    }
}

cv::Mat RealSenseVideo::getColorFeed() {
    return this->colorFeed;
}

cv::Mat RealSenseVideo::getDepthFeed() {
    return this->depthMeters;
}

cv::Mat RealSenseVideo::getMappedFeed() {
    return this->depthMeters;
}

inline void RealSenseVideo::update() {

    *vid >> colorFeed;
    if (!vidFolder) {
        *vidDepth >> depthFeed;
        depthMeters = depth_frame_to_meters(depthFeed);
    }
    else {
        std::string name = folder;
        name += std::to_string(currentDepth) + ".txt";
        this->LoadMatBinary(depthVideo +"/" + name,depthMeters);
    }

    currentDepth++;

    if (colorFeed.empty()) {
        vid->release();
        delete vid;
        currentDepth = 0;
        vid = new cv::VideoCapture(colorVideo);
        *vid >> colorFeed;
        if (vidDepth != NULL) {
        vidDepth->release();
        delete vidDepth;
        }
        if (!vidFolder) {
            vidDepth = new cv::VideoCapture(depthVideo);
        *vidDepth >> depthFeed;
        depthMeters = depth_frame_to_meters(depthFeed);
        } else {
            std::string name = folder;
            name += std::to_string(currentDepth) + ".txt";
            this->LoadMatBinary(depthVideo +"/" + name,depthMeters);

            exit(-1);
        }
    }
}


std::string RealSenseVideo::getTimeStamp()
{
    int current = (int) vid->get(cv::CAP_PROP_POS_FRAMES) / vid->get(cv::CAP_PROP_FPS);
    int total = (int) vid->get(cv::CAP_PROP_FRAME_COUNT) / vid->get(cv::CAP_PROP_FPS);

    std::stringstream currentSecond;
    currentSecond << std::setw(2) << std::setfill('0') << std::to_string(current%60);

    std::stringstream totalSecond;
    totalSecond << std::setw(2) << std::setfill('0') << std::to_string(total%60);

    return std::to_string(current/60) + ":" + currentSecond.str() + " / " + std::to_string(total/60) + ":" + totalSecond.str();
}


// Converts depth frame to a matrix of doubles with distances in meters
cv::Mat RealSenseVideo::depth_frame_to_meters(cv::Mat depthPic) {

#ifndef USE_KITCHEN
    depthPic.convertTo(depthPic, CV_64F);
    depthPic = depthPic * 0.00100000005;

#else

    cv::Mat temp(depthPic.rows,depthPic.cols,CV_64FC1);


    for (int i = 0; i < depthPic.rows;i++) {
        for (int j = 0; j < depthPic.cols; j++) {
            int grayscale = +(depthPic.at<uchar>(i,j));
            double dist = grayscale == 0 ? 0.0 : grayscale > 255 ? 4.0 : grayscale / 64;
            temp.at<double>(i, j) = dist;
        }
    }
    depthPic.release();
    depthPic = temp;

#endif


    return depthPic;

}
bool RealSenseVideo::readMatBinary(std::ifstream& ifs, cv::Mat& in_mat)
{
        using namespace cv;

    if(!ifs.is_open()){
        return false;
    }

    int rows, cols, type;
   // ifs.read((char*)(&rows), sizeof(int));
    //if(rows==0){
   //     return true;
   // }
    //ifs.read((char*)(&cols), sizeof(int));
    //ifs.read((char*)(&type), sizeof(int));

    in_mat.release();
    in_mat.create(720, 1280, CV_64F);
    ifs.read((char*)(in_mat.data), in_mat.elemSize() * in_mat.total());

    return true;
}


//! Load cv::Mat as binary
/*!
\param[in] filename filaname to load
\param[out] output loaded cv::Mat
*/
bool RealSenseVideo::LoadMatBinary(const std::string& filename, cv::Mat& output){
    std::ifstream ifs(filename, std::ios::binary);
    return readMatBinary(ifs, output);
}


