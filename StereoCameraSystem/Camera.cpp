#include "Camera.h"

Camera::Camera(int camID, int camBoardWidth, int camBoardHeight, float camSquareSize) {
    // Affectation aux attributs
    ID = camID;
    boardWidth = camBoardWidth;
    boardHeight = camBoardHeight;
    squareSize = camSquareSize;

    open();
}

void Camera::open() {
    // Ouverture de la caméra
    cap.open(ID);
    if (!cap.isOpened()) {
        std::cerr << "Erreur : Impossible d'ouvrir la caméra" << ID << "." << std::endl;
        exit(-1); // Ajouter une gestion d'erreur ici
    }
    std::cout << "Caméra" << ID << "ouverte avec succès !" << std::endl;
}

int Camera::calibrate() {
    std::vector<cv::Point2f> corners;
    std::vector<cv::Point3f> objp;

    // Préparation des points 3D du damier
    for (int i = 0; i < boardHeight; ++i) {
        for (int j = 0; j < boardWidth; ++j) {
            objp.push_back(cv::Point3f(j * squareSize, i * squareSize, 0.0f));
        }
    }

    cv::Mat frame, gray;
    bool found = false;
    int validImagesCaptured = 0; // Compteur d'images valides

    while (validImagesCaptured < 10) { // Prise d'images jusqu'à 10 images valides

        cap >> frame;
        imageSize = frame.size();

        if (frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        found = cv::findChessboardCorners(gray, cv::Size(boardWidth, boardHeight), corners,
            cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE);

        if (found) {
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
                cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001));
            cv::drawChessboardCorners(frame, cv::Size(boardWidth, boardHeight), corners, found);

            cornersSequence.push_back(corners);
            objectPoints.push_back(objp); // On stocke les points de l'objet

            validImagesCaptured++; // On incrémente le compteur seulement si l'échiquier est détecté
        }

        cv::imshow("Calibration", frame);
        if (cv::waitKey(30) >= 0) break;
    }

    // Vérification si au moins une image a été capturée
    if (cornersSequence.empty()) {
        std::cerr << "Erreur : Aucune image valide capturée pour la calibration." << std::endl;
        return -1;
    }

    // Calibration de la caméra
    std::vector<cv::Mat> rvecs, tvecs;
    cv::calibrateCamera(objectPoints, cornersSequence, gray.size(), cameraMatrix, distCoeffs, rvecs, tvecs);

    return 0;

}

cv::Mat Camera::getDistCoeffs() const {
    return distCoeffs;
}

cv::Mat Camera::getCameraMatrix() const {
    return cameraMatrix;
}

std::vector<std::vector<cv::Point3f>> Camera::getObjectPoints() const {
    return objectPoints;
}

std::vector<std::vector<cv::Point2f>> Camera::getImagePoints() const {
    return cornersSequence;
}

cv::Size Camera::getImageSize() const {
    return imageSize;
}
