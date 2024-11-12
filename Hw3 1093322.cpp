#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <sstream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    ifstream file(argv[1], ios::in);

    vector<string> cases;
    vector<vector<Point2f>> imagePoints;     // 2D points for each image
    vector<Point2f> corners;                 // Detected chessboard corners in each image
    vector<vector<Point3f>> objectPoints;    // 3D object points
    vector<Point3f> objP;                    // Object points for each chessboard

    // Define the 3D points for the chessboard corners **(9x6 grid)**
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 6; j++) {
            objP.push_back(Point3f(j, i, 0));
        }
    }

    Mat img, gray, output, D, cameraMatrix, mapx, mapy;

    while (!file.eof()) {
        string filePath;
        file >> filePath;
        img = imread(filePath, IMREAD_COLOR);
        cvtColor(img, gray, COLOR_BGR2GRAY);

        // Detect chessboard corners in the grayscale image
        bool found = findChessboardCorners(gray, Size(6, 9), corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
        
        if (found) {
            // Add the detected corners and object points for calibration
            imagePoints.push_back(corners);
            objectPoints.push_back(objP);
        }
    }

    vector<Mat> rvecs, tvecs;
    Mat Rr = Mat::eye(Size(3, 3), CV_32FC1);

    // Calibrate the camera using the 3D object points and 2D image points
    calibrateCamera(objectPoints, imagePoints, Size(480, 640), cameraMatrix, D, rvecs, tvecs);
    img = imread(argv[2], IMREAD_COLOR);
    // Generate the undistortion map using camera matrix and distortion coefficients
    initUndistortRectifyMap(cameraMatrix, D, Rr, cameraMatrix, img.size(), CV_32FC1, mapx, mapy);

    remap(img, output, mapx, mapy, INTER_CUBIC);
    imwrite(argv[3], output);

    return 0;
}
