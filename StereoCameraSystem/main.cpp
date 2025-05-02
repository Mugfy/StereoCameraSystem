#include "main.h"

int main(void) {

    int boardWidth = 9;
    int boardHeight = 7;
    float squareSize = 15.0f;

    Camera camera1(1, boardWidth, boardHeight, squareSize);
    Camera camera2(2, boardWidth, boardHeight, squareSize);
    camera1.calibrate();
    camera2.calibrate();

    StereoCamera stereo(camera1, camera2);
    stereo.calibrate();

    std::pair<cv::Mat, cv::Mat> rectified = rectify(stereo.getK1(), stereo.getK2(), stereo.getDC1(), stereo.getDC2(), stereo.getR(), stereo.getT());

    featureMatching(rectified.first, rectified.second);

    //void triang3DPoints(const std::vector<cv::DMatch>& goodMatches,const std::vector<cv::KeyPoint>& keypoints1,const std::vector<cv::KeyPoint>& keypoints2,const cv::Mat& P1,const cv::Mat& P2);

    return 0;
}
