#include <stdio.h>
#include <climits>
#include <ctime>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../lib/ppoint.hpp"
#include "../lib/affin.hpp"


void comment_timestamp(std::string comment) {
	time_t now = time(0);
	// char* dt = ctime(&now);
	tm *ltm = localtime(&now);
	std::cout << ltm->tm_hour << ':' << ltm->tm_min << ' ' << ltm->tm_sec << "\t" << comment << std::endl;
}


class SIFT {
public:
	cv::Mat_<unsigned char> img_char;
	cv::Mat_<unsigned char> img;
	cv::Mat_<double> img_d;

	cv::Mat_<double> img_fx2;
	cv::Mat_<double> img_fy2;
	cv::Mat_<double> img_fxy;

	std::string img_dir;
	std::string ext;
	std::string sig_str;

	void set_img(cv::Mat_<unsigned char> img) {
		this->img = img.clone();
		this->img_char = img.clone();
		// std::cout << img_char << std::endl;
		// this->img.convertTo(this->img_d, CV_64FC1);
		// this->img_d = img.clone();
	}

	double get_intensity(int y, int x) {
		double intensity = 0;	
		// std::cout << this->img_fx2 << std::endl;
		intensity = sqrt(this->img_fx2(y, x) + this->img_fy2(y, x));
		return intensity;
	}
	cv::Mat_<double> get_intensity_mat(cv::Mat_<unsigned char> src) {
		int rows = src.rows, cols = src.cols;
		// cv::Mat_<double> mat_intensity = cv::Mat::zeros(rows, cols, CV_64FC1);
		cv::Mat_<double> mat_intensity(rows, cols);
		for (int y=0; y < rows; y++) {
			for (int x=0; x < cols; x++) {
				// comment_timestamp("intensity");
				std::cout << rows << ' ' << cols << std::endl;
				mat_intensity[y][x] = get_intensity(y, x);
			}
		}
	}
	double get_gauss_val(int y, int x, double sigma) {
		double gauss = (1.0/(sigma*sigma*2.0*M_PI))*exp(-x*x/(2.0*sigma*sigma) ) ;
		return gauss;
	}
	double get_weight(int y, int x, double sigma) {
		// this->differentiate();
		double intensity = this->get_intensity(y, x);
		double gauss_val = this->get_gauss_val(y, x, sigma);

		double weight = intensity*gauss_val;
		return weight;
	}
	double get_radian() {
		double radian = 0;
		return radian;
	}
	// get image differentiation 
	// fx^2, fxfy, fyfx, fy^2
	std::vector<cv::Mat_<double> > differentiate() {
		std::vector<cv::Mat_<double>  > imgs;
		imgs = this->differentiate(this->img);
		return imgs;
	}
	std::vector<cv::Mat_<double> > differentiate(cv::Mat_<unsigned char> &img) {
		std::vector<cv::Mat_<double> > imgs;
		int cols = img.cols;
		int rows = img.rows;
		this->img_fx2 = cv::Mat::zeros(rows, cols, CV_64FC1);
		this->img_fy2 = cv::Mat::zeros(rows, cols, CV_64FC1);
		this->img_fxy = cv::Mat::zeros(rows, cols, CV_64FC1);

		// cv::Mat_<double> mat_fx2(rows, cols);
		// cv::Mat_<double> mat_fy2(rows, cols);
		// cv::Mat_<double> mat_fxy(rows, cols);
		std::cout << "rows=" << rows << "  cols="  << cols << std::endl;
		double fx, fy;
		for (int y=0; y<rows; y++) {
			for (int x=0; x<cols; x++) {
				fx = (get_img_val(y, x-1, img) - get_img_val(y, x+1, img) ) / 2; 
				fy = (get_img_val(y-1, x, img) - get_img_val(y+1, x, img) ) / 2; 

				this->img_fx2(y, x) = fx*fx;
				this->img_fy2(y, x) = fy*fy;
				this->img_fxy(y, x) = fx*fy;
			}
		}
		cv::imwrite(this->img_dir + "fx2" + this->ext, img_fx2);
		cv::imwrite(this->img_dir + "fy2" + this->ext, img_fy2);
		cv::imwrite(this->img_dir + "fxy" + this->ext, img_fxy);

		imgs.push_back(this->img_fx2);
		imgs.push_back(this->img_fy2);
		imgs.push_back(this->img_fxy);
		comment_timestamp("differentiate end" );
		return imgs;
	}

	double get_img_val(int y, int x, cv::Mat_<double>  img) {
		int cols = img.cols;	
		int rows = this->img.rows;
		if (x < 0) x= -x-1;
		if (x >= cols) x =  (cols-1) - (x - (cols-1));
		if (y < 0) y= -y-1;
		if (y >= rows) y =  (rows-1) - (y - (rows-1)) ;
		double val = img(y, x);
		return val;
	}

};


int main() {
	SIFT sift = SIFT();
	sift.img_dir = "img/";
	sift.ext = ".png";
	cv::Mat_<unsigned char> img = cv::imread(sift.img_dir + "Chessboard" + sift.ext, 0);
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

	for (int i= 0; i < 6; i++,  sigma += k) {
		std::stringstream ss;
		ss << sigma;
		sift.sig_str = ss.str();
		int N = sigma*3;
		cv::Mat_<unsigned char> feature_img =  cv::imread(sift.img_dir + "harris_laplacian1_" + sift.sig_str + sift.ext, 0);
		cv::Mat_<unsigned char> src_img = cv::imread(sift.img_dir + "gaussian" + sift.sig_str + sift.ext, 0);
		src_imgs.push_back(src_img);
		feature_imgs.push_back(feature_img);
	}
	for (int i=0; i < (int)src_imgs.size(); i++) {
		cv::Mat_<double>  intensity_mat;
		cv::Mat_<double>  radian_mat;
		cv::Mat_<double>  weight_mat;
		sift.set_img(src_imgs[i]);
		sift.differentiate(src_imgs[i]);
		intensity_mat = sift.get_intensity_mat(src_imgs[i]);
		cv::imwrite(sift.img_dir + "intensity" + sift.sig_str + sift.ext, intensity_mat ); 
	}

	/// gauss window

	///// weight
	double weights[rows][cols];
	for (int y=0; y<rows; y++) {
		for (int x=0; x<cols; x++) {
			weights[y][x] = sift.get_weight(y, x, sigma);
		}
	}


	//// hist
	// sift.get_hist();


	return 0;
}
