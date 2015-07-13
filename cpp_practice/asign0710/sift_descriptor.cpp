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

	cv::Mat_<double> mat_intensity;
	cv::Mat_<double> mat_radian;
	cv::Mat_<double> mat_weight;

	double sigma;

	std::string img_dir;
	std::string ext;
	std::string sig_str;

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
		this->mat_radian = cv::Mat::zeros(rows, cols, CV_64FC1);
		cv::Mat_<double> mat_radian(rows, cols);
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
		this->mat_weight = cv::Mat::zeros(rows, cols, CV_64FC1);
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
	/// gauss window
	sigma = 1;
	int hist_dim = 36;
	std::vector<cv::Mat_<unsigned char> > direction_mats;
	for (int scale=0; scale<(int)feature_imgs.size(); scale++, sigma+=k) {
		sift.set_sigma(sigma);
		// radian_mat = cv::imread(sift.img_dir + "radian" + sift.sig_str + sift.ext);
		// radian_mat = cv::imread(sift.img_dir + "weight" + sift.sig_str + sift.ext);
		int N = sigma*3;
		int nth = 0;
		for (int y=0; y < rows; y++) {
			for (int x=0; x < cols; x++) {
				if (feature_imgs[scale](y, x) == UCHAR_MAX) {
					nth ++;
					cv::Mat_<unsigned char> direction_mat = cv::Mat::zeros(rows, cols, CV_8UC1);
					comment_timestamp("scale="+std::to_string(scale)+' '+std::to_string(nth) + "th feature point");
					// cv::Mat_<unsigned char> hist = cv::Mat::zeros(2*N, 2*N, CV_64FC1);
					double hist[hist_dim];
					for (int i= -N; i<= N; i++) {
						for (int j= -N; j<= N; j++) {
							double theta = sift.get_img_val( y+i, x+j, radian_mats[scale]);
							// comment_timestamp("theta=" + std::to_string(theta) + ' ' + std::to_string(j));
							int direction = theta*(hist_dim/(2*M_PI)); ///// direction [0, 35]
							if (theta == 2*M_PI+1) continue;
							// if ( !(direction>=0 && direction<hist_dim)) assert("direction overflow" + std::to_string(direction));
							assert( (direction>=-1 && direction < hist_dim ));
							double weight = sift.get_img_val(y+i, x+j, intensity_mats[scale]) * sift.get_gauss_val(i,j,sigma);
							// comment_timestamp(std::to_string(i) + ' ' + std::to_string(j) + ' ' + std::to_string(direction));
							hist[direction] += weight;
							// std::cout << "direction=" << direction << " weight=" << hist[direction] << std::endl;
							// direction_mat[y+i][x+j] = direction*7;
						}
					}
					double max = -1;
					int major_direction = -1;
					for (int dir=0; dir<hist_dim; dir++) {
						// std::cout << hist[dir] << std::endl;
						if (hist[dir] > max) {
							max = hist[hist_dim];
							major_direction = dir;
						}
					}
					///// TODO major direction is not only one. 
					std::cout << "major direction = " <<  major_direction << " hist val=" << max << std::endl;
					// cv::imwrite(sift.img_dir + "direction" + sift.sig_str + "_"+std::to_string(nth) + sift.ext, direction_mat);

					///// sift descriptor


				}
			}
		}
	}

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
