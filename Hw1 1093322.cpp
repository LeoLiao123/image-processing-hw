#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>

using namespace cv;
using namespace std;

// Comparison function for sorting points
bool comp(Point a, Point b) {
    return (a.x == b.x) ? (a.y < b.y) : (a.x < b.x);
}

int main(int argc, char* argv[]) {
    ifstream file(argv[1], ios::binary);
    string line;
    vector<Point> A, B, C;  // Vectors to store points of each class
    Mat img = Mat::zeros(960, 960, CV_8UC3);
    Mat img2 = Mat::zeros(960, 960, CV_8UC3);
    Mat img3 = Mat::zeros(960, 960, CV_8UC3);

    while (getline(file, line)) {
        int classType = 0, startIdx = 0;

        // Determine class type by examining the line content
        if (line.find("A") != string::npos) classType = 1;
        else if (line.find("B") != string::npos) classType = 2;
        else if (line.find("C") != string::npos) classType = 3;

        // Parse points based on identified class type
        if (classType) {
            size_t start = line.find("[") + 1;
            while (start < line.size()) {
                size_t comma = line.find(",", start);
                size_t end = line.find_first_of(",]", comma + 1);
                
                int x = stoi(line.substr(start, comma - start));
                int y = stoi(line.substr(comma + 1, end - comma - 1));
                
                Point p(x, y);
                if (classType == 1) A.push_back(p);
                else if (classType == 2) B.push_back(p);
                else if (classType == 3) C.push_back(p);

                start = end + 1;
            }
        }

        // Draw lines based on class type and clear the vector after each draw
        if (classType == 1) { polylines(img, A, 1, Scalar(128, 0, 0), 1); A.clear(); }
        if (classType == 2) { polylines(img2, B, 1, Scalar(186, 85, 211), 1); B.clear(); }
        if (classType == 3) { polylines(img3, C, 1, Scalar(2, 0, 124), 1); C.clear(); }
    }

    vector<Point> AB, AC, BC;  // Store intersection points for each pair

    // Find intersections between A & B
    for (int i = 0; i < img.rows; i++) {
        uchar* rowA = img.ptr<uchar>(i);
        uchar* rowB = img2.ptr<uchar>(i);
        for (int j = 0; j < img.cols; j++) {
            if (rowA[j * 3] == 0 && rowA[j * 3 + 1] == 0 && rowA[j * 3 + 2] == 128 &&
                rowB[j * 3] == 211 && rowB[j * 3 + 1] == 85 && rowB[j * 3 + 2] == 186) {
                AB.push_back(Point(j, i));
            }
        }
    }

    // Find intersections between A & C
    for (int i = 0; i < img.rows; i++) {
        uchar* rowA = img.ptr<uchar>(i);
        uchar* rowC = img3.ptr<uchar>(i);
        for (int j = 0; j < img.cols; j++) {
            if (rowA[j * 3] == 0 && rowA[j * 3 + 1] == 0 && rowA[j * 3 + 2] == 128 &&
                rowC[j * 3] == 124 && rowC[j * 3 + 1] == 0 && rowC[j * 3 + 2] == 2) {
                AC.push_back(Point(j, i));
            }
        }
    }

    // Find intersections between B & C
    for (int i = 0; i < img.rows; i++) {
        uchar* rowB = img2.ptr<uchar>(i);
        uchar* rowC = img3.ptr<uchar>(i);
        for (int j = 0; j < img.cols; j++) {
            if (rowB[j * 3] == 211 && rowB[j * 3 + 1] == 85 && rowB[j * 3 + 2] == 186 &&
                rowC[j * 3] == 124 && rowC[j * 3 + 1] == 0 && rowC[j * 3 + 2] == 2) {
                BC.push_back(Point(j, i));
            }
        }
    }

    // Sort and output intersections
    sort(AB.begin(), AB.end(), comp);
    sort(AC.begin(), AC.end(), comp);
    sort(BC.begin(), BC.end(), comp);

    for (const auto& p : AB) cout << "A & B cross at: (" << p.x << ", " << p.y << ")\n";
    for (const auto& p : AC) cout << "A & C cross at: (" << p.x << ", " << p.y << ")\n";
    for (const auto& p : BC) cout << "B & C cross at: (" << p.x << ", " << p.y << ")\n";
}
