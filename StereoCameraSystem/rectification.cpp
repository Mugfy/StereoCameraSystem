#include "rectification.h"

int rectify(const cv::Mat& K1, const cv::Mat& K2, const cv::Mat& distCoeffs1, const cv::Mat& distCoeffs2) {
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

    // Détecter les points clés et calculer les descripteurs

    //Points clés = points facilement reconnassable (coins par exemple) pour ne pas etre sensible aux variations
    // Descripteur = vecteur des caratéristiques autour d'un point clé
    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    //fonction Opencv pour attribuer les descripteurs aux points clés
    orb->detectAndCompute(img1, cv::Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2);

    // Mettre en correspondance les points entre les deux images
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    // Trier les matches par distance (ordre croissant)
    // Plus la distance est faible entre deux vecteurs, plus les chances d'avoir un bon matches sont importantes
    // (utile pour la rectification)
    std::sort(matches.begin(), matches.end());

    // Extraire les points correspondants
    std::vector<cv::Point2f> points1, points2;
    for (const auto& match : matches) {
        points1.push_back(keypoints1[match.queryIdx].pt);
        points2.push_back(keypoints2[match.trainIdx].pt);
    }
    //debug
    std::cout << "Nombre de points correspondants : " << points1.size() << std::endl;

    // Calculer la matrice essentielle avce fonction classique d'OpenCv
    cv::Mat E = cv::findEssentialMat(points1, points2, K1, cv::RANSAC);
    std::cout << "Matrice essentielle :\n" << E << std::endl;

    // Récupérer la pose (R, T) à partir de la matrice essentielle
    // T : vecteur de translation entre les caméras
    // R : Rotation entre les deux caméras
    cv::Mat R, T;
    int numInliers = cv::recoverPose(E, points1, points2, R, T, K1);
    std::cout << "Matrice rotation R :\n" << R << std::endl;
    std::cout << "Translation T  :\n" << T << std::endl;

    // Calculer les matrices de rectification
    // Matrices R1 et R2 : Matrices de rotations des caméras
    // Matrices P1 et P2 : Matrices de projection 3D des caméras.
    cv::Mat R1, R2, P1, P2, Q;
    cv::stereoRectify(K1, distCoeffs1, K2, distCoeffs2, img1.size(), R, T, R1, R2, P1, P2, Q);

    //Affichage des atrices de rotation pour verification : 
    std::cout << "Matrice de rotation R \n : " << R << std::endl;
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