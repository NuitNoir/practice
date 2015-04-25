
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "drawLine.hpp"

void Hough(cv::Mat&);

int main() {
	cv::Mat src = cv::imread("building.jpg", 0);

	//src.channels() = 1;
	Hough(src);
}

void Hough(cv::Mat &src) {
	//cv::vector lines;
	int height = 320, width = 480;
	int x0, y0, x1, y1, x2, y2;
	cv::vector<cv::Vec2f> lines;
	cv::Mat dst, cdst;
	cv::Canny(src, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);
	//cv::Mat dst(src.rows, src.cols, CV_8UC1, 127);
	std::cout << lines.size() << std::endl;

  //imshow("source", src);

	cv::HoughLines(src, lines, 1.0, CV_PI/180, 150, 0, 0 );

	std::cout << lines.size() << std::endl;
	for (int i=0; i<(int)lines.size(); i++) {
		double rho = lines[i][0];
		double theta = lines[i][1];
		double tan = sin(theta)/cos(theta);
		x0 = rho*cos(theta);
		y0 = rho*sin(theta);
		x1 = x0 + y0*(1/tan);
		y1 = y0 + x0*(tan);
		x2 = x0 + (height - y1)*(1/tan);
		y2 = y0 + (width - x1)*(tan);

		draw_line(src, x1, y1, x2, y2, 128);
		//draw_line(src, lines[i][0], lines[i][1], lines[i][2], lines[i][3], 1);
	}

	//std::cout << dst.channels() << ' ' << dst.type() << ' '  << dst.cols << ' ' <<  dst.rows  << std::endl;
	cv::imwrite("hough.png", src);
}
