#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<algorithm>
#include<sstream>
using namespace cv;
using namespace std;
int main(int argc, char* argv[]) {
	ifstream file(argv[1], ios::in);
	vector<string> cases;
	vector<vector<Point2f>> Pointssssss;
	vector<Point2f> Points;
	vector< vector<cv::Point3f>> objPs;
	vector<cv::Point3f> objP;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 6; j++)
			objP.push_back(cv::Point3f(j, i, 0));
	Mat img, gray, output, D, camera, mapx, mapy;
	while (!file.eof()) {
		string s;
		file >> s;
		img = imread(s, IMREAD_COLOR);
		cvtColor(img, gray, COLOR_BGR2GRAY);
		findChessboardCorners(gray, cv::Size(6, 9), Points, CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
		Pointssssss.push_back(Points), objPs.push_back(objP);
	}
	vector<Mat> R, T;
	Mat Rr = Mat::eye(Size(3, 3), CV_32FC1);
	calibrateCamera(objPs, Pointssssss, cv::Size(480, 640), camera, D, R, T);
	img = imread(argv[2], IMREAD_COLOR);
	initUndistortRectifyMap(camera, D, Rr, camera, img.size(), CV_32FC1, mapx, mapy);
	remap(img, output, mapx, mapy, cv::INTER_CUBIC);
	imwrite(argv[3], output);
}