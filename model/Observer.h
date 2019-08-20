#pragma once

#include <opencv2/opencv.hpp>
/**
 * Interface for the Observer
 */
class Observer {

public:

    /**
     * Update the state of this observer
     * @param test entier de test
     */
    virtual void updateByObservee(cv::Mat imageColor,cv::Mat imageDepth) = 0;

};
