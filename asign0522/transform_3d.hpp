
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
#include "../asign0508/ppoint.hpp"

using namespace tutor;
using namespace cv;
using namespace std;

cv::Matx44d Translation(double tx, double ty, double tz) {
	Matx44d T (1, 0, 0, tx,
			0, 1, 0, ty,
			0, 0, 1, tz,
			0, 0, 0, 1);
	return T;
}

cv::Matx44d RotationX(double angle) {
	Matx44d R (1, 0, 0, 0,
			0, cos(angle), -sin(angle), 0,
			0, sin(angle), cos(angle), 0,
			0, 0, 0, 1);
	return R;
}

Matx44d RotationY(double angle) {
	Matx44d R (cos(angle), 0, sin(angle), 0,
			0, 1, 0, 0,
			-sin(angle), 0, cos(angle), 0,
			0, 0, 0, 1);
	return R;
}
cv::Matx44d RotationZ(double angle) {
	Matx44d R (cos(angle), -sin(angle), 0, 0,
			sin(angle), cos(angle), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	return R;
}
