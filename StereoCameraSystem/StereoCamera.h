#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

#include "Camera.h"

#include <vector>
#include <iostream>

class StereoCamera {

private:
    Camera cam1, cam2;
    cv::Mat R, T, E, F;
    cv::Mat R1, R2, P1, P2, Q;
    cv::Mat map1x, map1y, map2x, map2y;

public:
    StereoCamera(const Camera&, const Camera&);
    int calibrate();
    cv::Mat getR() const;
    cv::Mat getT() const;
    cv::Mat getE() const;
    cv::Mat getF() const;
};