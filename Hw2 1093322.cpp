#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<algorithm>
using namespace cv;
using namespace std;
bool PointCmp(const Point &a, const Point &b, const Point &center)
{
	if (a.x >= 0 && b.x < 0)
		return true;
	if (a.x == 0 && b.x == 0)
		return a.y > b.y;
	int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
	if (det < 0)
		return true;
	if (det > 0)
		return false;
	int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
	int d2 = (b.x - center.x) * (b.x - center.y) + (b.y - center.y) * (b.y - center.y);
	return d1 > d2;
}
void ClockwiseSortPoints(std::vector<Point> &vPoints)
{
	cv::Point center;
	int x = 0, y = 0;
	for (int i = 0; i < vPoints.size(); i++)
	{
		x += vPoints[i].x;
		y += vPoints[i].y;
	}
	center.x = (int)x / vPoints.size();
	center.y = (int)y / vPoints.size();

	for (int i = 0; i < vPoints.size() - 1; i++)
	{
		for (int j = 0; j < vPoints.size() - i - 1; j++)
		{
			if (PointCmp(vPoints[j], vPoints[j + 1], center))
			{
				cv::Point tmp = vPoints[j];
				vPoints[j] = vPoints[j + 1];
				vPoints[j + 1] = tmp;
			}
		}
	}
}
int cross(Point a, Point b) {
	Point o(0, 0);
	return (a.x - o.x)*(b.y - o.y) - (a.y - o.y)*(b.x - o.x);
}
Point intersection(Point a1, Point a2, Point b1, Point b2) {
	Point a(a2.x - a1.x, a2.y - a1.y);
	Point b(b2.x - b1.x, b2.y - b1.y);
	Point s(b1.x - a1.x, b1.y - a1.y);
	Point O(-1, -1);
	if (cross(a, b) == 0)
		return O;
	Point ans(a1.x + a.x*cross(s, b) / cross(a, b), a1.y + a.y* cross(s, b) / cross(a, b));
	return ans;
}
double det(vector<Point> PointVector) {
	double ans = 0;
	for (int i = 0; i < PointVector.size(); i++) {
		ans += (PointVector[i].x*PointVector[(i + 1) % PointVector.size()].y) - (PointVector[(i + 1) % PointVector.size()].x*PointVector[i].y);
	}
	ans = ans / 2;
	return round(abs(ans));
}
bool comp(Point a, Point b) {
	if (a.x == b.x) {
		return a.y < b.y;
	}
	return a.x < b.x;
}
int main(int argc, char* argv[]) {
	ifstream file(argv[1], ios::in);
	string s;
	vector<cv::Point> A1, compare;
	Point aa(523, 488);
	Point bb(431, 693);
	Point cc(816, 676);
	Point dd(683, 467);
	Point o(-1, -1);
	compare.push_back(aa), compare.push_back(bb), compare.push_back(cc), compare.push_back(dd);
	while (!file.eof()) {
		int ClassType = 0, startIdx = 0;
		file >> s;
		for (int i = 2; i < s.size(); i++) {                        
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
		}
		if (ClassType != 0) {                                       
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
					if (x > 959)
						x = 959;
					if (x < 0)
						x = 0;
					if (y > 959)
						y = 959;
					if (y < 0)
						y = 0;
					if (Xele != "}") {
						Point ele(x, y);
						if (ClassType == 1) {
							A1.push_back(ele);
						}
					}
				}
			}
		}
	}
	file.close();
	vector<Point> temp;
	vector<Point> A2(A1);
	Point check = A2[0];
	vector<Point> A2t(A2);
	ClockwiseSortPoints(A2);
	vector<Point> A(A2);
	if (A2t != A2) {
		A.clear();
		for (int i = A2t.size() - 1; i >= 0; i--) {
			A.push_back(A2t[i]);
		}
	}
	for (int i = 0; i < A.size(); i++) {
		Point a1 = A[i];
		Point a2 = A[(i + 1) % A.size()];
		for (int j = 0; j < compare.size(); j++) {
			Point b1 = compare[j];
			Point b2 = compare[(j + 1) % compare.size()];
			Point ele = intersection(a1, a2, b1, b2);
			if (ele != o && ele.x <= max(b1.x, b2.x) && ele.x >= min(b1.x, b2.x) && ele.y <= max(b1.y, b2.y) && ele.y >= min(b1.y, b2.y) &&
				ele.x <= max(a1.x, a2.x) && ele.x >= min(a1.x, a2.x) && ele.y <= max(a1.y, a2.y) && ele.y >= min(a1.y, a2.y)) {
				temp.push_back(ele);
			}
		}
	}
	for (int i = 0; i < A1.size(); i++) {
		if (pointPolygonTest(compare, A1[i], false) == 1)
			temp.push_back(A1[i]);
	}
	for (int i = 0; i < compare.size(); i++) {
		if (pointPolygonTest(A1, compare[i], false) == 1)
			temp.push_back(compare[i]);
	}
	vector<Point> hull(temp);
	ClockwiseSortPoints(hull);
	int AA = det(A1), Compare = det(compare);
	double a = det(hull);
	double ans = (a / (AA + Compare - a)) * 100;
	cout << "A's Area: " << AA << endl;
	cout << "X's Area: " << Compare << endl;
	cout << "Overlap rate: " << fixed << setprecision(3) << ans << "%" << endl;
}