//
// Created by uqamportable on 24/01/18.
//

#include "ImageSegmentationManual.h"

ImageSegmentationManual::ImageSegmentationManual() {


    filterUsed[BLUR] = true;
    filterUsed[BILFILTER] = false;
    filterUsed[HISTEQUAL] = true;
    filterUsed[CANNY] = false;
    filterUsed[THRESHOLD] = true;
    filterUsed[MEDIAN] = true;


}

ImageSegmentationManual::~ImageSegmentationManual() {

}


cv::Mat ImageSegmentationManual::segmentPhoto(cv::Mat picture){


}

void ImageSegmentationManual::setThreshhold(int threshMin, int threshMax) {
    minThresh = threshMin;
    maxThresh = threshMax;
}

cv::Mat ImageSegmentationManual::normalizePicHistogram(cv::Mat picture) const {

    if (picture.channels() >= 3)
    {
        cv::Mat ycrcb;

        cvtColor(picture, ycrcb, CV_BGR2YCrCb);
        std::vector<cv::Mat> channels;
        split(ycrcb, channels);

        equalizeHist(channels[0], channels[0]);
        equalizeHist(channels[1], channels[1]);
        equalizeHist(channels[2], channels[2]);

        cv::Mat result;
        merge(channels, ycrcb);

        cvtColor(ycrcb, result, CV_BGR2RGB);

        return result;
    }
    return picture;
}


cv::Mat ImageSegmentationManual::threshHoldColors(cv::Mat imgInput) const
{
    if (imgInput.channels() >= 3)
    {
        cv::Mat ycrcb;

        cvtColor(imgInput, ycrcb, CV_BGR2YCrCb);
        std::vector<cv::Mat> channels;
        split(ycrcb, channels);


        threshold(channels[0], channels[0], 120, 250, cv::THRESH_BINARY);
        threshold(channels[2], channels[2], 120, 250, cv::THRESH_BINARY);

        cv::Mat result;
        merge(channels, ycrcb);

        cvtColor(ycrcb, result, CV_BGR2RGB);

        return result;
    }
    return imgInput;
}





cv::Mat ImageSegmentationManual::segmentPic(cv::Mat picture) {


    cv::Mat threshold_output;

    std::vector<std::vector<cv::Point> > contours;

    std::vector<cv::Vec4i> hierarchy;

    cv::Mat filteredPic = picture.clone();
    cv::Mat filteredGreyPic;


    if (USECOLORTHREASH)
    	filteredPic = threshHoldColors(filteredPic);

    if (USEHISTEQUAL)
        filteredPic = normalizePicHistogram(filteredPic);

    cvtColor(filteredPic, filteredGreyPic, cv::COLOR_BGR2GRAY);
    //equalizeHist(filteredGreyPic, filteredGreyPic);

    //threshold(filteredGreyPic, threshold_output, minThresh, maxThresh, cv::THRESH_BINARY);

    Canny(filteredGreyPic, threshold_output, 50, 150, 3);


    findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));
    vector<Vec3f> circles;

    HoughCircles(threshold_output, circles, HOUGH_GRADIENT, 1, 10,
                 100, 30, 1, 100); // change the last two parameters
    // (min_radius & max_radius) to detect larger circles	for (size_t i = 0; i < lines.size(); i++)
    cv::Mat currentPicWithShapes = picture.clone();
    cv::Mat filteredPicWithShapes = threshold_output.clone();
    for (size_t i = 0; i < circles.size(); i++)
    {
        Vec3i c = circles[i];
        circle(currentPicWithShapes, Point(c[0], c[1]), c[2], Scalar(100, 100, 0), 3, LINE_AA);
        circle(currentPicWithShapes, Point(c[0], c[1]), 2, Scalar(100, 100, 0), 3, LINE_AA);
    }
    vector<Vec4i> lines;
    HoughLinesP(threshold_output, lines, 1, CV_PI / 180, 40, 40, 10);
    for (size_t i = 0; i < lines.size(); i++)
    {
        Vec4i l = lines[i];
        line(currentPicWithShapes, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(50, 0, 155), 3, CV_AA);
    }

    std::vector<std::vector<cv::Point> > contours_poly(contours.size());

    //![allthework]

    std::vector<cv::Rect> boundRect(contours.size());

    for (size_t i = 0; i < contours.size(); i++)
    {

        approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
        cv::Rect bounding = cv::boundingRect(cv::Mat(contours_poly[i]));
        if (bounding.area()  < (0.6 * (filteredGreyPic.cols * filteredGreyPic.rows)))
            boundRect[i] = bounding;



    }
    std::list<cv::Rect> boundRectNew;
    for (int i = 0; i < boundRect.size(); i++)
        if (boundRect[i].area() > 10)
            boundRectNew.push_back(boundRect[i]);

    bool verif = true;

    int c = 0;

    list<Rect>::iterator it = boundRectNew.begin();

    while (it != boundRectNew.end()) {
        verif = false;
        list<Rect>::iterator itt = it;
        itt++;
        while (itt != boundRectNew.end()) {
            if ((*it & *itt).area() > 0) {
                Rect temp = *it | *itt;
                boundRectNew.remove(*itt);
                boundRectNew.remove(*it);
                boundRectNew.push_front(temp);
                it = boundRectNew.begin();
                verif = true;
                itt = it;
                itt++;
                continue;
            }
            else
                itt++;
        }
        if (!verif)
            it++;
        std::cout << " The size : " << boundRectNew.size();

    }

    for (it = boundRectNew.begin(); it != boundRectNew.end(); it++) {
        currentShapes.push_back(picture(*it));
    }

    //filteredPicWithShapes = filteredPic.clone();
    //currentPicWithShapes = currentPic.clone();
    for (it = boundRectNew.begin(); it != boundRectNew.end();++it)
    {
        Scalar color = Scalar(0, 200, 0);

        rectangle(filteredPicWithShapes, it->tl(), it->br(), color, 2, 8, 0);
        //rectangle(currentPicWithShapes, it->tl(), it->br(), color, 2, 8, 0);

    }
    Scalar color = Scalar(0, 200, 0);

    for (int i = 0; i < boundRect.size(); i++) {

        if (hierarchy[i][2] + hierarchy[i][3] < 0)
        {
            rectangle(currentPicWithShapes, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
            putText(currentPicWithShapes, hierarchy[i][0] + " " + hierarchy[i][1], boundRect[i].tl(), cv::HersheyFonts::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(140, 0, 0));
        }
    }

    return currentPicWithShapes;


}
