#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

#include <vector>
#include <iostream>

class Camera {

private:
    int ID;
    int boardWidth, boardHeight;
    float squareSize;

    cv::VideoCapture cap;
    cv::Size imageSize;
    cv::Mat cameraMatrix, distCoeffs;
    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<std::vector<cv::Point2f>> cornersSequence;

public:
    Camera(int camID, int camBoardWidth, int camBoardHeight, float camSquareSize);
    void open(); // Ouverture de la caméra
    int calibrate(); // Calibration de la caméra
    cv::Mat getDistCoeffs() const; // Getter pour les coefficients de distorsion
    cv::Mat getCameraMatrix() const; // Getter pour la matrice de la caméra
    std::vector<std::vector<cv::Point3f>> getObjectPoints() const; // Getter pour les points objet
    std::vector<std::vector<cv::Point2f>> getImagePoints() const; // Getter pour les points image
    cv::Size getImageSize() const; // Getter pour la taille de l'image
};
