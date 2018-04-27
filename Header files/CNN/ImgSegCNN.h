#pragma once
#include "CNN.h"
#include <opencv2/dnn.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/core/core.hpp>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/trace.hpp>
using namespace cv;
using namespace cv::dnn;

#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

class ImgSegCNN : public  CNN {
public:
    ImgSegCNN();
    ~ImgSegCNN();

    void train();
    std::string getPictureInfo(const cv::Mat& image);
    void updateModel(const cv::Mat& picture, bool correctlyIdentified);
    std::string predict(const cv::Mat& picture);
    void savePicture(const cv::Mat& picture, std::string name);
    std::vector<double> getClassesProb(const Mat &probBloc) ;

private:
    std::string lastName;
    float lastScore;
    cv::dnn::Net neuralNet;
    void getMaxClass(const Mat &probBlob, int *classId, double *classProb);
    std::vector<String> readClassNames(const char *filename );
    cv::Mat mean_;
    cv::Scalar channel_mean;
    int check;
};