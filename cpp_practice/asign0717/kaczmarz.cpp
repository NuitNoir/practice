#include <stdio.h>
#include <climits>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

void comment_timestamp(std::string comment) {
	time_t now = time(0);
	// char* dt = ctime(&now);
	tm *ltm = localtime(&now);
	std::cout << ltm->tm_hour << ':' << ltm->tm_min << ' ' << ltm->tm_sec << "\t" << comment << std::endl;
}

class KACZMARZ {
public:
	int N ;
	cv::Mat_<double> mat_A;	
	cv::Mat_<double> vec_b;
	cv::Mat_<double> vec_x;
	cv::Mat_<unsigned char> img;

	KACZMARZ() {
		int n = 2500;
		int m = 2700;
		this->vec_x = cv::Mat::zeros(n, 1, CV_64FC1);		
		this->vec_b = cv::Mat::zeros(m, 1, CV_64FC1);
		this->mat_A = cv::Mat::zeros(m, n, CV_64FC1);
	}

	cv::Mat_<double> set_mat_A(std::string filename) {
		int rows = this->mat_A.rows, cols = this->mat_A.cols;
		std::ifstream ifs(filename.c_str());
		double val = 0;
		for (int i=0; !ifs.eof(); i++) {
			ifs >> val;
			// if (val != 0) std::cout << val << ' ' << i/rows << ' ' << i%rows <<  std::endl;
			this->mat_A(i/rows, i%rows)  = val;
		}
		// std::cout << this->mat_A << std::endl;
		comment_timestamp("set mat_A end");
		return this->mat_A;
	}
	cv::Mat_<double> set_vec_b(std::string filename) {
		int rows = vec_b.rows;
		// this->vec_b = cv::Mat::zeros(rows, cols, CV_64FC1);
		std::ifstream ifs(filename.c_str());
		double val;
		for (int i=0; !ifs.eof(); i++) {
			ifs >> val;
			// if (val != 0) std::cout << val << std::endl;
			this->vec_b(i/rows, i%rows) = val;
		}
		comment_timestamp("set vec_b end");
		return this->vec_b;
	}

	double vec_length(cv::Mat_<double> vec) {
		double length = 0;
		for (int i=0; i<vec.rows; i++) {
			length += vec(i, 0)*vec(i, 0);
		}
		// length = sqrt(length);
		return length;
	}

	double get_coeff(int i) {
		double bi = this->vec_b(i, 0);
		cv::Mat_<double> ai = this->mat_A.row(i);
		ai = ai.t();
		double ai_len2 = this->vec_length(ai);
		if (ai_len2 == 0) return false;
		// std::cout << "i=" << i <<  " ai.rows=" << ai.rows << " ai.cols=" << ai.cols << std::endl;
		double coeff = (bi -  ai.dot(vec_x) ) / (ai_len2);
		// std::cout << "i=" << i << "\tcoeff=" <<  coeff << "    \tai*x=" << ai.dot(vec_x) << "  \tai_len=" << ai_len2 << "\tbi=" << bi << std::endl;
		return coeff;
	}

	////// regular  i = k mod m+1 = k mod A.rows + 1
	cv::Mat_<double> regular_kaczmarz() {
		int rows = this->mat_A.rows;
		int n = 2500;
		// this->vec_x = cv::Mat::zeros(n, 1, CV_64FC1);
		// int cols = this->mat_A.cols;
		for (int k=0; k < this->N*rows; k++) {
			int i = k%(rows);
			// std::cout << this->mat_A.row(i);
			cv::Mat_<double> ai = this->mat_A.row(i);
			ai = ai.t();
			// std::cout << "i=" << i <<  " vec_x.rows=" <<vec_x.rows << " vec_x.cols=" <<vec_x.cols << std::endl;
			if (this->get_coeff(i) == false) continue;
			this->vec_x = this->vec_x + 	this->get_coeff(i) * ai;
			// std::cout << "i=" << i <<  " vec_x.rows=" <<vec_x.rows << " vec_x.cols=" <<vec_x.cols << std::endl;
		}
		comment_timestamp("end regular kaczmarz");
		return this->vec_x;
	}

	///// symmetric  i =  1, 2, ... , m-1, m, m-1, ... 2, 1
	cv::Mat_<double> symmmetric_kaczmarz() {
		int rows = this->mat_A.rows;
		for (int k=0; k<this->N*rows; k++) {
			int i = k;
			if (k > rows) i = 2*rows - k;

		}
		comment_timestamp("end regular kaczmarz");
		return this->vec_x;
	}

	///// randomize  i = r(i)
	cv::Mat_<double> randomize_kaczmarz() {
		return this->vec_x;
	}
};

int main() {
	int imrow = 50, imcol = 50;
	KACZMARZ kacz = KACZMARZ();
	kacz.N = 10;
	///// A.shape = 2700x2500 = mxn
	///// 2500 = 50x50 image size
	///// 2700 = 75x180[dig]/5 
	kacz.set_mat_A("data/kaczmarz/mat_A.dat");
	///// b.shape = 2700x1 = mx1
	kacz.set_vec_b("data/kaczmarz/vector_b_ex.dat");
	///// x.shape = 2500x1 = A.colsx1 = nx1
	kacz.vec_x = cv::Mat::zeros(kacz.mat_A.cols, 1, CV_64FC1);
	cv::Mat_<unsigned char> img = cv::Mat::zeros(50, 50, CV_8UC1);

	kacz.regular_kaczmarz();

	int n = kacz.vec_x.rows;
	for (int i=0; i<n; i++) {
		img(i/imrow, i%imrow) = (unsigned char)kacz.vec_x(i, 0);
	}
	cv::imwrite("image.png", img);
	return 0;
}

