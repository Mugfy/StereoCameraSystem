#include "rectification.h"

int rectify(const cv::Mat& K1, const cv::Mat& K2, const cv::Mat& distCoeffs1, const cv::Mat& distCoeffs2, const cv::Mat& R, const cv::Mat& T) {
    // Ouvrir les caméras
    cv::VideoCapture cap1(1); // Caméra sur le port USB 1
    cv::VideoCapture cap2(2); // Caméra sur le port USB 2


    if (!cap1.isOpened() || !cap2.isOpened()) {
        std::cerr << "Erreur : Impossible d'ouvrir les caméras pour la rectification stéréo." << std::endl;
        return -1;
    }

    // Lire une image de chaque caméra
    cv::Mat img1, img2;
    cap1 >> img1;
    std::cout << "Image avec la caméra 1 prise" << std::endl;
    cap2 >> img2;
    std::cout << "Image avec la caméra 2 prise" << std::endl;
    // Calculer les matrices de rectification
    // Matrices R1 et R2 : Matrices de rotations des caméras
    // Matrices P1 et P2 : Matrices de projection 3D des caméras.
    cv::Mat R1, R2, P1, P2, Q;
    cv::stereoRectify(K1, distCoeffs1, K2, distCoeffs2, img1.size(), R, T, R1, R2, P1, P2, Q);

    //Affichage des atrices de rotation pour verification : 
    std::cout << "Matrice de rotation R1 \n : " << R1 << std::endl;
    std::cout << "Matrice de rotation R2 \n : " << R2 << std::endl;

    // Calculer les mappings pour la rectification
    cv::Mat map1x, map1y, map2x, map2y;
    cv::initUndistortRectifyMap(K1, distCoeffs1, R1, P1, img1.size(), CV_32FC1, map1x, map1y);
    cv::initUndistortRectifyMap(K2, distCoeffs2, R2, P2, img2.size(), CV_32FC1, map2x, map2y);

    // Rectifier les images
    cv::Mat img1Rectified, img2Rectified;
    cv::remap(img1, img1Rectified, map1x, map1y, cv::INTER_LINEAR);
    cv::remap(img2, img2Rectified, map2x, map2y, cv::INTER_LINEAR);

    // Afficher les images rectifiées
    cv::imshow("Image Rectifiée Caméra 1", img1Rectified);
    cv::imshow("Image Rectifiée Caméra 2", img2Rectified);
    cv::waitKey(0);

    return 0;
}
