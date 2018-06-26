//
// Created by uqamportable on 11/05/18.
//

#include "RealSenseVideo.h"

//
// Created by uqamportable on 16/02/18.
//


RealSenseVideo::RealSenseVideo() {

    //rs2::config cfg;

    Gtk::FileChooserDialog dialog("Please select a color video",
                                  Gtk::FILE_CHOOSER_ACTION_OPEN);


    dialog.add_button("Select", Gtk::RESPONSE_OK);

    int result = dialog.run();

    //Handle the response:
    switch (result) {
        case (Gtk::RESPONSE_OK): {
            colorVideo = dialog.get_filename();
            break;
        }

        default: {
            exit(-1);
        }
    }


    vid = new cv::VideoCapture(colorVideo);

    dialog.hide();
    Gtk::FileChooserDialog dialog2("Please select a depth video",
                                  Gtk::FILE_CHOOSER_ACTION_OPEN);


    //Add response buttons the the dialog:
    dialog2.add_button("Select", Gtk::RESPONSE_OK);

     result = dialog2.run();

    //Handle the response:
    switch (result) {
        case (Gtk::RESPONSE_OK): {
            depthVideo = dialog2.get_filename();
            break;
        }

        default: {
            exit(-1);
        }
    }
    vidDepth = new cv::VideoCapture(depthVideo);
    dialog2.hide();
}

RealSenseVideo::~RealSenseVideo() {
    vid->release();
    vidDepth->release();
    delete vid;
    delete vidDepth;
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


    //depthMeters = this->depth_frame_to_meters(pipe,depth);
    *vid >> colorFeed;
    *vidDepth >> depthFeed;
    depthMeters = depth_frame_to_meters(depthFeed);

    if (colorFeed.empty()) {
        vid->release();
        delete vid;
        vid = new cv::VideoCapture(colorVideo);
        *vid >> colorFeed;
        vidDepth->release();
        delete vidDepth;
        vidDepth = new cv::VideoCapture(depthVideo);
        *vidDepth >> depthFeed;
        depthMeters = depth_frame_to_meters(depthFeed);


    }



}



// Converts depth frame to a matrix of doubles with distances in meters
cv::Mat RealSenseVideo::depth_frame_to_meters(cv::Mat depthPic) {

    cv::Mat temp(depthFeed.rows,depthFeed.cols,CV_64FC1);


    for (int i = 0; i < depthFeed.rows;i++) {
        for (int j = 0; j < depthFeed.cols; j++) {
            int grayscale = +(depthFeed.at<uchar>(i,j));
            double dist = grayscale == 0 ? 0.0 : grayscale > 255 ? 4.0 : grayscale / 64;
            temp.at<double>(i, j) = dist;
        }
    }

    return temp;
}

