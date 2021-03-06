/*
 * PGM.cpp
 *
 *  Created on: 2015/04/10
 *      Author: kyota
 */

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
//#include <opencv2/cv.hpp>

using namespace std;
void Gauss(cv::Mat);
void Laplace(cv::Mat);
void Binalize(cv::Mat);
void OtsuBinalize(cv::Mat);

int main() {
	cv::Mat src = cv::imread("cat/cat1.pgm", 1);
	//cv::imwrite("out_cat1.pgm", img);
	//cv::Mat dst = cv::imwrite("out_cat1.pgm", src);

	//Gauss(src);
	//Laplace(src);
	Binalize(src);
	OtsuBinalize(src);
}

void Gauss(cv::Mat src) {
	cv::Mat dst(src.rows, src.cols, CV_8UC1, 0);
	GaussianBlur(src, dst,cv::Size(5,5), 10, 10, 0);
	cv::imwrite("gauss.pgm", dst);
}

void Laplace(cv::Mat src) {
	cv::Mat dst(src.rows, src.cols, CV_8UC1, 0);
	int ksize = 9;
	int scale = 3;
	int delta = 5;
	Laplacian(src, dst, src.depth(), ksize, scale, delta, 0);
	cv::imwrite("laplace.pgm", dst);
}

void Binalize(cv::Mat src) {
	cv::Mat dst(src.rows, src.cols, src.type(), 0);

	cv::threshold(src, dst, 128, 255, CV_THRESH_BINARY);

	cv::imwrite("binary.pgm", dst);
}

void OtsuBinalize(cv::Mat src) {
	cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
	cv::Mat dst(src.rows, src.cols, src.type(), 0);

	cv::threshold(src, dst, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	cv::imwrite("otsu_binary.pgm", dst);
}
