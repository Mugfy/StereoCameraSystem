#include "StereoCamera.h"

StereoCamera::StereoCamera(const Camera& c1, const Camera& c2)
    : cam1(c1), cam2(c2) {}

int StereoCamera::calibrate() {

    std::vector<std::vector<cv::Point3f>> objectPoints = cam1.getObjectPoints();
    std::vector<std::vector<cv::Point2f>> imgPoints1 = cam1.getImagePoints();
    std::vector<std::vector<cv::Point2f>> imgPoints2 = cam2.getImagePoints();
    cv::Size imageSize = cam1.getImageSize();

    cv::stereoCalibrate(objectPoints, imgPoints1, imgPoints2,
        cam1.getCameraMatrix(), cam1.getDistCoeffs(),
        cam2.getCameraMatrix(), cam2.getDistCoeffs(),
        imageSize, R, T, E, F,
        cv::CALIB_FIX_INTRINSIC);

    return 0;
}

cv::Mat StereoCamera::getR() const {
    return R;
}

cv::Mat StereoCamera::getT() const {
    return T;
}

cv::Mat StereoCamera::getE() const {
    return E;
}

cv::Mat StereoCamera::getF() const {
    return F;
}