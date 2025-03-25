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

    return 0;
}
