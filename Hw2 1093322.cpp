#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace cv;
using namespace std;

// Compare two points based on their angle with respect to a center point, used for sorting in a clockwise direction
bool PointCmp(const Point &a, const Point &b, const Point &center) {
    if (a.x >= 0 && b.x < 0) return true;
    if (a.x == 0 && b.x == 0) return a.y > b.y;

    int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
    if (det < 0) return true;
    if (det > 0) return false;

    int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
    int d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
    return d1 > d2;
}

// Sort points in a clockwise direction around their center
void ClockwiseSortPoints(vector<Point> &vPoints) {
    Point center;
    int x = 0, y = 0;
    for (const auto& pt : vPoints) {
        x += pt.x;
        y += pt.y;
    }
    center.x = x / vPoints.size();
    center.y = y / vPoints.size();

    // Bubble sort to arrange points in a clockwise order around the center
    for (int i = 0; i < vPoints.size() - 1; i++) {
        for (int j = 0; j < vPoints.size() - i - 1; j++) {
            if (PointCmp(vPoints[j], vPoints[j + 1], center)) {
                swap(vPoints[j], vPoints[j + 1]);
            }
        }
    }
}

// Calculate cross product to determine relative orientation
int cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

// Find intersection point of two line segments, returns (-1, -1) if no intersection
Point intersection(Point a1, Point a2, Point b1, Point b2) {
    Point a(a2.x - a1.x, a2.y - a1.y);
    Point b(b2.x - b1.x, b2.y - b1.y);
    Point s(b1.x - a1.x, b1.y - a1.y);
    Point noIntersection(-1, -1);

    if (cross(a, b) == 0) return noIntersection;
    Point intersect(a1.x + a.x * cross(s, b) / cross(a, b), a1.y + a.y * cross(s, b) / cross(a, b));
    return intersect;
}

// Calculate the area of a polygon using the determinant method
double det(const vector<Point>& PointVector) {
    double area = 0;
    for (int i = 0; i < PointVector.size(); i++) {
        area += (PointVector[i].x * PointVector[(i + 1) % PointVector.size()].y) -
                (PointVector[(i + 1) % PointVector.size()].x * PointVector[i].y);
    }
    return round(abs(area) / 2);
}

// Compare two points for sorting
bool comp(Point a, Point b) {
    return (a.x == b.x) ? (a.y < b.y) : (a.x < b.x);
}

int main(int argc, char* argv[]) {
    ifstream file(argv[1], ios::in);
    string s;
    vector<Point> A1, comparePoints = { Point(523, 488), Point(431, 693), Point(816, 676), Point(683, 467) };
    Point noIntersection(-1, -1);

    // Read points from the input file
    while (file >> s) {
        if (s.find("A") != string::npos) {
            size_t startIdx = s.find("[") + 1;
            while (startIdx < s.size()) {
                size_t comma = s.find(",", startIdx);
                size_t end = s.find_first_of(",]", comma + 1);

                int x = stoi(s.substr(startIdx, comma - startIdx));
                int y = stoi(s.substr(comma + 1, end - comma - 1));

                A1.emplace_back(clamp(x, 0, 959), clamp(y, 0, 959));  // Add points within bounds
                startIdx = end + 1;
            }
        }
    }
    file.close();

    vector<Point> intersectionPoints;

    // Sort points of A1 in clockwise order
    ClockwiseSortPoints(A1);
    vector<Point> hullPoints(A1);

    // Detect intersection points between polygons A1 and comparePoints
    for (int i = 0; i < hullPoints.size(); i++) {
        Point a1 = hullPoints[i];
        Point a2 = hullPoints[(i + 1) % hullPoints.size()];
        for (int j = 0; j < comparePoints.size(); j++) {
            Point b1 = comparePoints[j];
            Point b2 = comparePoints[(j + 1) % comparePoints.size()];
            Point interPoint = intersection(a1, a2, b1, b2);

            if (interPoint != noIntersection && interPoint.x >= min(a1.x, a2.x) && interPoint.x <= max(a1.x, a2.x) &&
                interPoint.y >= min(a1.y, a2.y) && interPoint.y <= max(a1.y, a2.y)) {
                intersectionPoints.push_back(interPoint);
            }
        }
    }

    // Collect interior points for both polygons
    for (const auto& pt : A1) if (pointPolygonTest(comparePoints, pt, false) == 1) intersectionPoints.push_back(pt);
    for (const auto& pt : comparePoints) if (pointPolygonTest(A1, pt, false) == 1) intersectionPoints.push_back(pt);

    // Calculate the convex hull of the intersection points
    ClockwiseSortPoints(intersectionPoints);

    // Compute areas and overlap ratio
    int areaA = det(A1), areaCompare = det(comparePoints);
    double overlapArea = det(intersectionPoints);
    double overlapRate = (overlapArea / (areaA + areaCompare - overlapArea)) * 100;

    cout << "A's Area: " << areaA << endl;
    cout << "Compare's Area: " << areaCompare << endl;
    cout << "Overlap rate: " << fixed << setprecision(3) << overlapRate << "%" << endl;
}
