
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

void Hough(cv::Mat);
int main() {
	cv::Mat src = cv::imread("cat/cat1.pgm", 1);
	//src.channels() = 1;
	Hough(src);
}

void Hough(cv::Mat src) {
	cv::Mat dst(src.rows, src.cols, CV_8UC1, 255);
	cv::HoughLines(src, dst, 3.0,  CV_PI/180, 150, 0, 0);
	//GaussianBlur(src, dst,cv::Size(5,5), 10, 10, 0);
	cv::imwrite("hough.pgm", dst);
}
