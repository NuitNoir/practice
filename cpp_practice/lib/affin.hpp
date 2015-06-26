#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
#include "../asign0501/ppoint.hpp"

using namespace tutor;
using namespace cv;
using namespace std;

// Nearest Neighbour
unsigned char NN(Mat &src, double x, double y);
unsigned char Bilinear(Mat &src, double x, double y);


// Making Matrix
cv::Matx33d Rotation(double angle);
cv::Matx33d Translation(double, double);
cv::Matx33d Scaling(double sx, double sy);
cv::Matx33d Skew(double dx, double dy);
cv::Matx33d invRotation(double angle);
cv::Matx33d invTranslation(double, double);
cv::Matx33d invScaling(double sx, double sy);
cv::Matx33d invSkew(double dx, double dy);

// Image transform
cv::Mat TransformL(cv::Mat &src, const cv::Matx33d &M);
cv::Mat TransformE(cv::Mat &src, const cv::Matx33d &M, int flag);

class AffinTransformer {
public:
	cv::Mat TransformL(cv::Mat &src, const cv::Matx33d &M) {
		// cv::Mat dst = Mat::zeros(src.rows, src.cols, CV_8U);
		cv::Mat dst = Mat::zeros(src.rows, src.cols, CV_8U);

		PPoint2d p1(0, 0);
		PPoint2d p(0, 0);
		for (int y = 0; y < dst.rows; y++) {
			for (int x = 0; x < dst.cols; x++) {
				// Lagrange
				PPoint2d p1(x, y);
				p = M * p1;
				if (p1[0] < 0 || p1[0] >= src.cols || p1[1] < 0 || p1[1]>=src.rows) continue;
				unsigned char val = NN(src, p1[0], p1[1]);
				// std::cout << p1 << val << std::endl;
				if (p[1] < dst.rows && p[0] < dst.cols && p1[0] >= 0 && p1[1] >= 0)
					dst.at<unsigned char>(p[1], p[0]) = val;
			}
		}
		return dst;
	}

	cv::Mat TransformE(cv::Mat &src, const cv::Matx33d &M, int flag) {
		cv::Mat dst = Mat::zeros(src.rows, src.cols, CV_8U);
		unsigned char val;
		PPoint2d p1(0, 0);
		PPoint2d p(0, 0);
		for (int y = 0; y < dst.rows; y++) {
			for (int x = 0; x < dst.cols; x++) {
				// Euler
				PPoint2d p1(x, y);
				p = M * p1;
				if (flag == 0) {
					val = NN(src, p[0], p[1]);
				} else if (flag == 1) {
					val = Bilinear(src, p[0], p[1]);
				} else {
					cout << "invalid flag = " << flag << endl;
					cout
					<< "flag must be 0:Nearest Neighbour, 1:Bilinear Interpolation"
					<< endl;
					break;
				}
				if (y < dst.rows && x < dst.cols && y >= 0 && x >= 0)
					dst.at<unsigned char>(p1[1], p1[0]) = val;
			}
		}
		return dst;
	}

	unsigned char NN(Mat &src, double x, double y) {
		int ix, iy;
		ix = (int) (x + 0.5);
		iy = (int) (y + 0.5);
		if (y < src.rows && x < src.cols && y >= 0 && x >= 0) {
			return src.at<unsigned char>(iy, ix);
		} else {
			return 0;
		}
	}

	unsigned char Bilinear(Mat &src, double x, double y) {
		int ix = (int) x;
		int iy = (int) y;
		int diff_x, diff_y;
		unsigned char val;

		diff_x = x - ix;
		diff_y = y - iy;

		if (y < src.rows && x < src.cols && y >= 0 && x >= 0) {
			val =
			(1 - diff_y)
			* ((1 - diff_x) * src.at<unsigned char>(ix, iy)
				+ diff_x * src.at<unsigned char>(ix + 1, iy))
			+ diff_y
			* ((1 - diff_x)
				* src.at<unsigned char>(ix, iy + 1)
				+ diff_x
				* src.at<unsigned char>(ix + 1,
					iy + 1));
			return val;
		} else {
			return 0;
		}

	}

	cv::Matx33d Rotation(double theta) {
		Matx33d R(cos(theta), -sin(theta), 0, sin(theta), cos(theta), 0, 0, 0, 1);
		return R;
	}
	cv::Matx33d Translation(double tx, double ty) {
		Matx33d T(1, 0, tx, 0, 1, ty, 0, 0, 1);
		return T;
	}
	cv::Matx33d Scaling(double sx, double sy) {
		Matx33d S(sx, 0, 0, 0, sy, 0, 0, 0, 1);
		return S;
	}
	cv::Matx33d Skew(double dx, double dy) {
		Matx33d S(1, dx, 0, dy, 1, 0, 0, 0, 1);
		return S;
	}
	// inverse matrix
	cv::Matx33d invRotation(double theta) {
		Matx33d R(cos(theta), sin(theta), 0, -sin(theta), cos(theta), 0, 0, 0, 1);
		return R;
	}
	cv::Matx33d invTranslation(double tx, double ty) {
		Matx33d T(1, 0, -tx, 0, 1, -ty, 0, 0, 1);
		return T;
	}
	cv::Matx33d invScaling(double sx, double sy) {
		Matx33d S(1 / sx, 0, 0, 0, 1 / sy, 0, 0, 0, 1);
		return S;
	}
	cv::Matx33d invSkew(double dx, double dy) {
		Matx33d S(1, -dx, 0, -dy, 1, 0, 0, 0, 1);
		return S;
	}
};

/*void dst_mat_size(void) {
 PPoint2d p1(0,0);
 PPoint2d p2(src.cols, 0);
 PPoint2d p3(0, src.rows);
 PPoint2d p4(src.cols, src.rows);
 int minx= 10000, miny = 1000, maxx = -1000, maxy = -1000;
 PPoint2d  p(0,0);
 cv::Matx33d R = Rotation(theta);
 cv::Matx33d T1 = Translation(-src.cols/2, src.rows/2);
 cv::Matx33d T2 = Translation(src.cols/2, -src.rows/2);


 //point 1


 p = T2*R*T1*p1;
 std::cout << p << endl;
 if (p[0] < minx) {minx = (double)p[0];}
 else if (p[0] > maxx) { maxx = p[0]; }
 if (p[1] < miny) {miny = p[1];}
 else if (p[1] > maxy) {maxy = p[1];}
 //point 2
 p = T2*R*T1*p2;
 std::cout << p << endl;
 if (p[0] < minx) {minx = (double)p[0];}
 else if (p[0] > maxx) { maxx = p[0]; }
 if (p[1] < miny) {miny = p[1];}
 else if (p[1] > maxy) {maxy = p[1];}
 //point 3
 p = T2*R*T1*p3;
 std::cout << p << endl;
 if (p[0] < minx) {minx = (double)p[0];}
 else if (p[0] > maxx) { maxx = p[0]; }
 if (p[1] < miny) {miny = p[1];}
 else if (p[1] > maxy) {maxy = p[1];}
 //point 4
 p = T2*R*T1*p4;
 std::cout << p << endl;
 if (p[0] < minx) {minx = (double)p[0];}
 else if (p[0] > maxx) { maxx = p[0]; }
 if (p[1] < miny) {miny = p[1];}
 else if (p[1] > maxy) {maxy = p[1];}

 cout << minx <<' '<< miny <<' '<< maxx <<' '<< maxy << endl;
 //dst.cols = 10000;// maxx - minx;
 //dst.rows = 10000; //maxy - miny;
 cout << dst.rows <<' '<< dst.cols << endl;
 }*/
