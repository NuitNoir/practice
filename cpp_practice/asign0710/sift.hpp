#include <stdio.h>
#include <climits>
#include <ctime>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


class SIFT {
public:
	cv::Mat_<unsigned char> src;
	cv::Mat_<unsigned char> img_char;
	cv::Mat_<unsigned char> img;
	cv::Mat_<double> img_d;

	cv::Mat_<double> img_fx2;
	cv::Mat_<double> img_fy2;
	cv::Mat_<double> img_fxy;

	cv::Mat_<double> mat_intensity;
	cv::Mat_<double> mat_radian;
	cv::Mat_<double> mat_weight;

	double sigma;

	std::string img_dir;
	std::string ext;
	std::string sig_str;

	SIFT(cv::Mat_<unsigned char> img) {
		this->img = img.clone();
		this->src = img.clone();
		this->img_char = img.clone();
		this->img.convertTo(this->img_d, CV_64FC1);	
		
		int rows = this->img.rows, cols = this->img.cols;
		this->mat_radian = cv::Mat::zeros(rows, cols, CV_64FC1);
		this->mat_weight = cv::Mat::zeros(rows, cols, CV_64FC1);

	}

	void set_img(cv::Mat_<unsigned char> img) {
		this->img = img.clone();
		this->img_char = img.clone();
		this->img.convertTo(this->img_d, CV_64FC1);
	}

	double get_intensity(int y, int x) {
		double intensity = 0;	
		intensity = sqrt(this->img_fx2(y, x) + this->img_fy2(y, x));
		// std::cout << "intensity=" << intensity << std::endl;
		return intensity;
	}
	cv::Mat_<double> get_intensity_mat() {
		int rows = this->img.rows, cols = this->img.cols;
		// cv::Mat_<double> mat_intensity(rows, cols);
		this->mat_intensity = cv::Mat::zeros(rows, cols, CV_64FC1);
		for (int y=0; y < rows; y++) {
			for (int x=0; x < cols; x++) {
				this->mat_intensity[y][x] = get_intensity(y, x);
			}
		}
		return this->mat_intensity;
	}

	double get_radian(int y, int x) {
		double radian = 0;
		double fx = sqrt(this->img_fx2(y,x));
		double fy = sqrt(this->img_fy2(y,x));
		radian = atan(fy/fx);
		if (fx == 0) radian = 2*M_PI + 1;
		// if (fx != 0 && fy != 0) std::cout << "radian=" << radian << " fx=" << fx << " fy=" << fy <<  std::endl;
		return radian;
	}
	cv::Mat_<double> get_radian_mat() {
		int rows = this->img.rows, cols = this->img.cols;
		for (int y=0; y < rows; y++) {
			for (int x=0; x < cols; x++) {
				this->mat_radian[y][x] = get_radian(y, x);
			}
		}
		return this->mat_radian;
	}

	cv::Mat_<double> get_weight_mat(double sigma) {
		int rows = this->img.rows, cols = this->img.cols;
		// cv::Mat_<double> mat_weight(rows, cols);
		for (int y=0; y < rows; y++) {
			for (int x=0; x < cols; x++) {
				this->mat_weight[y][x] = get_weight(y, x, sigma);
			}
		}
		return this->mat_weight;
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

	cv::Mat_<double> get_affine_mat(double theta) {
		cv::Mat_<double> R = (cv::Mat_<double>(2, 2) << cos(theta), sin(theta),
						-sin(theta), cos(theta) );
		return R;
	}
	///// return 0...7
	int get_direction(cv::Mat_<unsigned char> img, int y, int x) {


	}
	cv::Mat_<unsigned char> rotate(double angle, int y, int x) {
	    	int rows = this->src.rows, cols = this->src.cols;

	    	cv::Point2f pt(y, x);
	    	cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);

	    	cv::warpAffine(this->src, this->img, r, cv::Size(rows, cols));
	    	return this->img;
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
	void set_sigma(double sigma) {
		std::stringstream ss;
		ss << sigma;
		this->sig_str = ss.str();
	}

};