#include "calibration.h"

int calibrate(void) {

    // Ouvrir la caméra USB (0 est généralement l'ID de la première caméra)
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Erreur : Impossible d'ouvrir la caméra." << std::endl;
        return -1;
    }

    // Paramètres du damier
    int boardWidth = 9;  // Nombre de coins horizontaux
    int boardHeight = 6; // Nombre de coins verticaux
    float squareSize = 25.0f; // Taille d'un carré en mm

    std::vector<cv::Point2f> corners;
    std::vector<std::vector<cv::Point2f>> cornersSequence;
    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<cv::Point3f> objp;

    // Préparer les points 3D du damier
    for (int i = 0; i < boardHeight; ++i) {
        std::vector <cv::Point3f> objp(boardWidth);

        for (int j = 0; j < boardWidth; ++j) {
            objp.push_back(cv::Point3f(j * squareSize, i * squareSize, 0.0f));
        }

        objectPoints.push_back(objp);


    }

    /*for (int i = 0; i < boardHeight; ++i) {
        for (int j = 0; j < boardWidth; ++j) {
            objp.push_back(cv::Point3f(j * squareSize, i * squareSize, 0.0f));
        }
    }*/

    cv::Mat frame, gray;
    bool found = false;

    // Capturer des images pour la calibration
    while (cornersSequence.size() < 10) { // Capturer au moins 10 images
        cap >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        found = cv::findChessboardCorners(gray, cv::Size(boardWidth, boardHeight), corners,
            cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE);

        if (found) {
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
                cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001));
            cv::drawChessboardCorners(frame, cv::Size(boardWidth, boardHeight), corners, found);

            cornersSequence.push_back(corners);
            objectPoints.push_back(objp); // Correction : on ajoute objp et non objectPoints
        }

        cv::imshow("Calibration", frame);
        if (cv::waitKey(30) >= 0) break;
    }

    // Calibration de la caméra
    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;
    cv::calibrateCamera(objectPoints, cornersSequence, gray.size(), cameraMatrix, distCoeffs, rvecs, tvecs);

    std::cout << "Matrice de calibration : " << cameraMatrix << std::endl;
    std::cout << "Coefficients de distorsion : " << distCoeffs << std::endl;

	return 0;
}