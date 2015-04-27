
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "drawLine.hpp"

void Hough(cv::Mat&);

int main() {
	cv::Mat src = cv::imread("building.jpg", 0);
	//cv::Mat src = cv::imread("line.png", 0);
	//src.channels() = 1;
	Hough(src);
}

void Hough(cv::Mat &src) {
	//cv::vector lines;
	int height = src.rows, width = src.cols;
	//int x0, y0, x1, y1, x2, y2;
	cv::vector<cv::Vec2f> lines;
	cv::Mat dst, cdst;
	cv::Canny(src, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);
	//cv::Mat dst(src.rows, src.cols, CV_8UC1, 127);
	std::cout << lines.size() << std::endl;

  //imshow("source", src);

	cv::HoughLines(src, lines, 15.0, CV_PI/180, 150);

	std::cout << lines.size() << std::endl;
	for (int i=0; i<(int)lines.size(); i++) {
		double r = lines[i][0];
		double theta = lines[i][1];
		double tan = sin(theta)/cos(theta);
		cv::Point p1;
		cv::Point p2;
		p1.x = r/cos(theta);
		p1.y = 0;
		p2.x = 0;
		p2.y = r/sin(theta);

		//std::cout << lines[i] << std::endl;
		//std::cout << x1 << ' ' << y1 << ' '<< x2<<' ' << y2<<' ' << std::endl;
		cv::line(src, p1, p2, 10);
		//draw_line(src, x1, y1, x2, y2, 1);
		//draw_line(src, lines[i][0], lines[i][1], lines[i][2], lines[i][3], 1);
	}

	//std::cout << dst.channels() << ' ' << dst.type() << ' '  << dst.cols << ' ' <<  dst.rows  << std::endl;
	cv::imwrite("hough.png", src);
}
