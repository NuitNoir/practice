#include <stdio.h>
#include <climits>
#include <ctime>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "comment_timestamp.hpp"
#include "../lib/ppoint.hpp"
#include "../lib/affin.hpp"		
#include "sift.hpp"


int main() {
	cv::Mat_<unsigned char> img = cv::imread("img/Chessboard.png", 0);
	SIFT sift = SIFT(img);
	sift.img_dir = "img/";
	sift.ext = ".png";
	cv::Mat_<double> img_d = cv::imread(sift.img_dir + "Chessboard" + sift.ext, 0);

	sift.set_img(img);
	comment_timestamp(sift.img_dir + "Chessboard " + sift.ext);

	std::vector<cv::Mat_<double> > diff_imgs;
	int rows = sift.img.rows, cols = sift.img.cols;
	double k = 1.2;
	double sigma = 1 + k;

	std::vector<cv::Mat_<unsigned char> > src_imgs;
	std::vector<cv::Mat_<unsigned char> > feature_imgs;
	
	std::vector<cv::Mat_<double> > intensity_mats;
	std::vector<cv::Mat_<double> > radian_mats;
	std::vector<cv::Mat_<double> > weight_mats;

	///// read images
	for (int i= 0; i < 5; i++,  sigma += k) {
		sift.set_sigma(sigma);
		int N = sigma*3;
		cv::Mat_<unsigned char> feature_img =  cv::imread(sift.img_dir + "harris_laplacian1_" + sift.sig_str + sift.ext, 0);
		cv::Mat_<unsigned char> src_img = cv::imread(sift.img_dir + "gaussian" + sift.sig_str + sift.ext, 0);
		src_imgs.push_back(src_img);
		feature_imgs.push_back(feature_img);
	}
	comment_timestamp("image read end");
	///// intensity, radian, weight calculation
	sigma = 1;
	for (int i=0; i < (int)src_imgs.size(); i++, sigma+=k) {
		sift.set_img(src_imgs[i]);
		sift.set_sigma(sigma);
		// sift.differentiate(src_imgs[i]);
		cv::Mat_<double> img_fx2 = cv::imread(sift.img_dir + "fx2" + sift.sig_str + sift.ext, 0);
		cv::Mat_<double> img_fy2 = cv::imread(sift.img_dir + "fy2" + sift.sig_str + sift.ext, 0);
		comment_timestamp(sift.img_dir + "fx2" + sift.sig_str + sift.ext);
		sift.img_fx2 = img_fx2;
		sift.img_fy2 = img_fy2;
		cv::Mat_<double> intensity_mat = sift.get_intensity_mat();
		cv::Mat_<double> radian_mat = sift.get_radian_mat();
		// std::cout << radian_mat << std::endl;
		// weight_mat = 	sift.get_weight_mat(sigma);
		cv::imwrite(sift.img_dir + "intensity" + sift.sig_str + sift.ext, intensity_mat ); 
		cv::imwrite(sift.img_dir + "radian" + sift.sig_str + sift.ext, intensity_mat ); 
		intensity_mats.push_back(intensity_mat);
		radian_mats.push_back(radian_mat);
		// cv::imwrite(sift.img_dir + "weight" + sift.sig_str + sift.ext, intensity_mat ); 
	}
	comment_timestamp("intensity, radian, weight calculation end");



	/////////////////////////////////////////// gauss window
	std::vector<cv::Mat_<double> > sift_descriptors;
	std::vector<cv::Mat_<unsigned char> > descriptors;
	sigma = 1;
	// std::vector<cv::Mat_<unsigned char> > direction_mats;
	cv::Mat::_<unsigned char> direction_mat;
	for (int scale=0; scale<(int)feature_imgs.size(); scale++, sigma+=k) {
		sift.set_sigma(sigma);
		int N = sigma*3;
		int nth = 0;
		for (int y=0; y < rows; y++) {
			for (int x=0; x < cols; x++) {
				if (feature_imgs[scale](y, x) != UCHAR_MAX) continue;
				nth ++;
				int major_direction = sift.get_major_direction(y, x, N, radian_mats[scale], intensity_mats[scale]);
				///// TODO major direction is not only one. 
				std::cout << "major direction = " <<  major_direction << std::endl;
				sift.get_sift_descriptor();
			}
		}
	}	



	//// compare sift_descriptors
	for (int i=0; i<(int)sift_descriptors.size(); i++) {
	}


	return 0;
}
