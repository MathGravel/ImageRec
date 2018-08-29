#include "sources/RealSenseVideo.h"


RealSenseVideo::RealSenseVideo(std::string colorFile ,std::string depthFile ) : colorVideo(std::move(colorFile)),depthVideo(std::move(depthFile)) {

    vid = new cv::VideoCapture(colorVideo);
    if (depthVideo.back() != 't') {
        vidDepth = new cv::VideoCapture(depthVideo);
        vidFolder = false;
        folder = "";
    }
    else
    {
       depthVideo = depthVideo.substr(0, depthVideo.size()-7);
       vidDepth = new cv::VideoCapture(depthVideo + "/%03d.txt");
       vidFolder = true;
       folder = depthVideo;
    }

}

RealSenseVideo::~RealSenseVideo() {
    vid->release();
    delete vid;
    if (vidDepth != NULL) {
        vidDepth->release();
        delete vidDepth;
    }
}

cv::Mat RealSenseVideo::getColorFeed() {
    return this->colorFeed;
}

cv::Mat RealSenseVideo::getDepthFeed() {
    return this->depthFeed;
}

cv::Mat RealSenseVideo::getMappedFeed() {
    return this->depthMeters;
}

void RealSenseVideo::update() {

    *vid >> colorFeed;
    *vidDepth >> depthFeed;

    // Pour accélérer la vidéo...
    /*
    *vid >> colorFeed;
    *vidDepth >> depthFeed;
    *vid >> colorFeed;
    *vidDepth >> depthFeed;
    *vid >> colorFeed;
    *vidDepth >> depthFeed;
    */

    depthMeters = depth_frame_to_meters(depthFeed);
    depthMeters = colorFeed;
    if (colorFeed.empty()) {
        vid->release();
        delete vid;
        vid = new cv::VideoCapture(colorVideo);
        *vid >> colorFeed;
        vidDepth->release();
        delete vidDepth;
        if (vidFolder)
            vidDepth = new cv::VideoCapture(depthVideo + "/%03d.txt");
        else
            vidDepth = new cv::VideoCapture(depthVideo);
        *vidDepth >> depthFeed;
        depthMeters = depth_frame_to_meters(depthFeed);
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

    depthPic = depthPic * 0.00100000005;



    return depthPic;
   /*if (vidFolder) {
        using namespace cv;
        using namespace rs2;

        depthPic.convertTo(depthPic, CV_64F);
        auto depth_scale = pipe.get_active_profile()
                .get_device()
                .first<depth_sensor>()
                .get_depth_scale();
        depthPic = depthPic * depth_scale;



        return depthPic;


    }*/


    cv::Mat temp(depthPic.rows,depthPic.cols,CV_64FC1);


    for (int i = 0; i < depthPic.rows;i++) {
        for (int j = 0; j < depthPic.cols; j++) {
            int grayscale = +(depthPic.at<uchar>(i,j));
            double dist = grayscale == 0 ? 0.0 : grayscale > 255 ? 4.0 : grayscale / 64;
            temp.at<double>(i, j) = dist;
        }
    }

    return temp;
}
bool RealSenseVideo::readMatBinary(std::ifstream& ifs, cv::Mat& in_mat)
{
        using namespace cv;

    if(!ifs.is_open()){
        return false;
    }

    int rows, cols, type;
    ifs.read((char*)(&rows), sizeof(int));
    if(rows==0){
        return true;
    }
    ifs.read((char*)(&cols), sizeof(int));
    ifs.read((char*)(&type), sizeof(int));

    in_mat.release();
    in_mat.create(rows, cols, type);
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

