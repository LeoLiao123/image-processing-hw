#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>
#include<fstream>
using namespace cv;
using namespace std;
bool comp(Point a, Point b) {
	if (a.x == b.x) {
		return a.y < b.y;
	}
	return a.x < b.x;
}
int main(int argc,char* argv[]) {
	ifstream file(argv[1], ios::binary);
	string s;
	vector<cv::Point> A, B, C;
	Mat img = Mat::zeros(960, 960, CV_8UC3);
	Mat img2 = Mat::zeros(960, 960, CV_8UC3);
	Mat img3 = Mat::zeros(960, 960, CV_8UC3);
	while (!file.eof()) {
		int ClassType = 0, startIdx = 0;
		file >> s;
		for (int i = 2; i < s.size(); i++) {                        //分辨class name
			if (s[i] == ','&&s[i - 2] == 'A') {
				ClassType = 1;
				for (int j = i; j < s.size(); j++) {
					if (s[j] == '[') {
						startIdx = j + 1;
						break;
					}
				}
				break;
			}
			else if (s[i] == ','&&s[i - 2] == 'B') {
				ClassType = 2;
				for (int j = i; j < s.size(); j++) {
					if (s[j] == '[') {
						startIdx = j + 1;
						break;
					}
				}
				break;
			}
			else if (s[i] == ','&&s[i - 2] == 'C') {
				ClassType = 3;
				for (int j = i; j < s.size(); j++) {
					if (s[j] == '[') {
						startIdx = j + 1;
						break;
					}
				}
				break;
			}
		}
		if (ClassType != 0) {                                       //有讀到class name就讀接下來的陣列
			int start = startIdx, end;
			for (int i = startIdx; i < s.size(); i++) {
				if (s[i] == ',') {
					end = i;
					string Xele = s.substr(start, end - start);
					start = end + 1;
					for (int j = start; j < s.size(); j++) {
						if (s[j] == ',' || s[j] == ']') {
							end = j;
							break;
						}
					}
					string Yele = s.substr(start, end - start);
					start = end + 1;
					i = end + 1;
					int x = atoi(Xele.c_str());
					int y = atoi(Yele.c_str());
					if (Xele != "}") {
						Point ele(x, y);
						if (ClassType == 1) {
							A.push_back(ele);
						}
						else if (ClassType == 2) {
							B.push_back(ele);
						}
						else if (ClassType == 3) {
							C.push_back(ele);
						}
					}
				}
			}
		}
		if (ClassType == 1) {                                                                         //依Class Name來繪圖 每次畫完都清空 不然會造成測資沒分割好  多出不必要的直線
			polylines(img, A, 1, CV_RGB(128, 0, 0), 1, 8, 0);
			A.clear();
		}
		else if (ClassType == 2) {
			polylines(img2, B, 1, CV_RGB(186, 85, 211), 1, 8, 0);
			B.clear();
		}
		else if (ClassType == 3) {
			polylines(img3, C, 1, CV_RGB(2, 0, 124), 1, 8, 0);
			C.clear();
		}
	}
	vector<cv::Point> AB,AC,BC;                                                                       //判斷每張畫布有象素點的位置  同個座標代表有交界點

	for (int i = 0; i < img.rows; i++) {                                                              //A & B
		uchar* Arows = img.ptr<uchar>(i);
		uchar* Brows = img2.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++) {
			int Ar = Arows[j * 3 + 2];
			int Ag = Arows[j * 3 + 1];
			int Ab = Arows[j * 3];

			int Br = Brows[j * 3 + 2];
			int Bg = Brows[j * 3 + 1];
			int Bb = Brows[j * 3];

			if ((Ar == 128 && Ag == 0 && Ab == 0) && (Br == 186 && Bg == 85 && Bb == 211)) {
				if (i > 5 && j > 5) {
					Point ele(j, i);
					AB.push_back(ele);
				}
			}
		}
	}
	for (int i = 0; i < img.rows; i++) {                                                              //A & C
		uchar* Arows = img.ptr<uchar>(i);
		uchar* Crows = img3.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++) {
			int Ar = Arows[j * 3 + 2];
			int Ag = Arows[j * 3 + 1];
			int Ab = Arows[j * 3];

			int Cr = Crows[j * 3 + 2];
			int Cg = Crows[j * 3 + 1];
			int Cb = Crows[j * 3];

			if ((Ar == 128 && Ag == 0 && Ab == 0) && (Cr == 2 && Cg == 0 && Cb == 124)) {
				if (i > 5 && j > 5) {
					Point ele(j, i);
					AC.push_back(ele);
				}
			}
		}
	}
	for (int i = 0; i < img.rows; i++) {                                                              //B & C
		uchar* Brows = img2.ptr<uchar>(i);
		uchar* Crows = img3.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++) {
			int Br = Brows[j * 3 + 2];
			int Bg = Brows[j * 3 + 1];
			int Bb = Brows[j * 3];

			int Cr = Crows[j * 3 + 2];
			int Cg = Crows[j * 3 + 1];
			int Cb = Crows[j * 3];

			if ((Br == 186 && Bg == 85 && Bb == 211) && (Cr == 2 && Cg == 0 && Cb == 124)) {
				if (i > 5 && j > 5) {
					Point ele(j, i);
					BC.push_back(ele);
				}
			}
		}
	}

	sort(AB.begin(), AB.end(), comp);                                                               //依題意排列輸出順序
	sort(AC.begin(), AC.end(), comp);
	sort(BC.begin(), BC.end(), comp);

	for (int i = 0; i < AB.size(); i++) {
		cout << "A & B cross at: (" << AB[i].x << ", " << AB[i].y << ")" << endl;
	}
	for (int i = 0; i < AC.size(); i++) {
		cout << "A & C cross at: (" << AC[i].x << ", " << AC[i].y << ")" << endl;
	}
	for (int i = 0; i < BC.size(); i++) {
		cout << "B & C cross at: (" << BC[i].x << ", " << BC[i].y << ")" << endl;
	}
}