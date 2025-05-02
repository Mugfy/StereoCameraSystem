#pragma once

#include <opencv2/opencv.hpp>
//#include <opencv2/viz.hpp>
#include <vector>

void triang3DPoints(
    const std::vector<cv::DMatch>& goodMatches,
    const std::vector<cv::KeyPoint>& keypoints1,
    const std::vector<cv::KeyPoint>& keypoints2,
    const cv::Mat& P1,
    const cv::Mat& P2);