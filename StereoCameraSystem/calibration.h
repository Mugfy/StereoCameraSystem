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
    cv::Mat cameraMatrix, distCoeffs;

public:
    Camera(int camID, int camBoardWidth, int camBoardHeight, float camSquareSize);
    void open(); // Ouverture de la caméra
    int calibrate(); // Calibration de la caméra
    cv::Mat getDistCoeffs() const; // Getter pour les coefficients de distorsion
    cv::Mat getCameraMatrix() const; // Getter pour la matrice de la caméra

};
