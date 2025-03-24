#include "main.h"

int main(void) {

    std::cout << "Hello World" << std::endl;

    int boardWidth = 9;
    int boardHeight = 7;
    float squareSize = 15.0f;

    Camera camera1(1, boardWidth, boardHeight, squareSize);
    Camera camera2(2, boardWidth, boardHeight, squareSize);

    camera1.open();
    camera1.calibrate();

    camera2.open();
    camera2.calibrate();

    std::cout << "Matrice de calibration :\n" << camera1.getCameraMatrix() << std::endl;
    std::cout << "Coefficients de distorsion :\n" << camera1.getDistCoeffs() << std::endl;

    std::cout << "Matrice de calibration :\n" << camera2.getCameraMatrix() << std::endl;
    std::cout << "Coefficients de distorsion :\n" << camera2.getDistCoeffs() << std::endl;

    return 0;
}
