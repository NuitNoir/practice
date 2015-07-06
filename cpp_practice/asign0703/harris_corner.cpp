#include <stdio.h>
#include <climits>
#include <ctime>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../lib/ppoint.hpp"
// #include "../lib/drawLine.hpp"

void differentiate(cv::Mat_<int> &img) ;

class Harris {
public:
	std::string img_dir;
	std::string sig_str;
	std::string ext;
	cv::Mat_<double> harris_operator(cv::Mat_<double> img, double sigma) {
		// cv::Mat_<double> img = cv::imread("Chessboard.png", 0);

		// std::string img_dir = "img/";
		// std::string sig_str = std::to_string(sigma);
		// std::string ext = ".png";
		std::vector<cv::Mat_<double> > diff_imgs;
		diff_imgs = differentiate(img); //// imgs = [fx2, fy2, fxy]
		cv::imwrite(this->img_dir + "fx2" + this->sig_str + this->ext, diff_imgs[0]);
		cv::imwrite(this->img_dir + "fy2" + this->sig_str + this->ext, diff_imgs[1]);
		cv::imwrite(this->img_dir + "fxy" + this->sig_str + this->ext, diff_imgs[2]);

		///// gauss 
		// cv::Mat_<double> fx2 = cv::imread(this->img_dir + "fx2.png", 0);
		// cv::Mat_<double> fy2 = cv::imread(this->img_dir + "fy2.png", 0);
		// cv::Mat_<double> fxy = cv::imread(this->img_dir + "fxy.png", 0);
		// std::string this->sig_str = "";
		// sprintf(this->sig_str, "%f.1", sigma);
		cv::Mat_<double> g_fx2 = gaussian_filter(diff_imgs[0], sigma);
		comment_timestamp("gaussian fx2 end");
		cv::Mat_<double> g_fy2 = gaussian_filter(diff_imgs[1], sigma);
		comment_timestamp("gaussian fy2 end");
		cv::Mat_<double> g_fxy = gaussian_filter(diff_imgs[2], sigma);
		comment_timestamp("gaussian fxy end");
		cv::imwrite(this->img_dir + "gauss_fx2" + this->sig_str + this->ext, g_fx2);		
		cv::imwrite(this->img_dir + "gauss_fy2" + this->sig_str + this->ext, g_fy2);	
		cv::imwrite(this->img_dir + "gauss_fxy" + this->sig_str + this->ext, g_fxy);	
		comment_timestamp("gaussian get end");

		///// response
		// cv::Mat_<double> g_fx2 = cv::imread(this->img_dir + "gauss_fx2" + this->sig_str + this->ext, 0);		
		// cv::Mat_<double> g_fy2 = cv::imread(this->img_dir + "gauss_fy2" + this->sig_str + this->ext, 0);		
		// cv::Mat_<double> g_fxy = cv::imread(this->img_dir + "gauss_fxy" + this->sig_str + this->ext, 0);		
		cv::Mat_<double> res = response(g_fx2, g_fy2, g_fxy);
		cv::imwrite(this->img_dir + "response" + this->sig_str + this->ext, res);
		comment_timestamp("response end");
		// maximum
		// cv::Mat_<double> res = cv::imread(this->img_dir + "res.png", 0);
		
		cv::Mat_<unsigned char> corner = maximum(res);
		cv::imwrite(this->img_dir + "feature_points"+this->sig_str+this->ext, corner);
		comment_timestamp("faeture points end");
		// mark src img
		cv::Mat_<unsigned char> src = cv::imread(this->img_dir + "Chessboard.png", 2);
		cv::Mat_<unsigned char> marked_src = mark_corner(src, corner);
		cv::imwrite(this->img_dir + "marked_feature_points"+this->sig_str+this->ext, marked_src);


		return corner;
	}


	// get image differentiation 
	// fx^2, fxfy, fyfx, fy^2
	std::vector<cv::Mat_<double> > differentiate(cv::Mat_<double> &img) {
		std::vector<cv::Mat_<double> > imgs;
		int cols = img.cols;
		int rows = img.rows;
		cv::Mat_<double> mat_fx2(rows, cols);
		cv::Mat_<double> mat_fy2(rows, cols);
		cv::Mat_<double> mat_fxy(rows, cols);

		double fx, fy;
		int x_1, x_2, y_1, y_2;

		for (int y=0; y<rows; y++) {
			for (int x=0; x<cols; x++) {
				fx = (get_img_val(y, x-1, img) - get_img_val(y, x+1, img) ) / 2;
				fy = (get_img_val(y-1, x, img) - get_img_val(y+1, x, img) ) / 2;

				mat_fx2(y, x) = fx*fx;
				mat_fy2(y, x) = fy*fy;
				mat_fxy(y, x) = fx*fy;
			}
		}
		// std::cout << mat_fx << std::endl;
		imgs.push_back(mat_fx2);
		imgs.push_back(mat_fy2);
		imgs.push_back(mat_fxy);
		// cv::imwrite(this->img_dir + "fx2.png", mat_fx2);
		// cv::imwrite(this->img_dir + "fy2.png", mat_fy2);
		// cv::imwrite(this->img_dir + "fxy.png", mat_fxy);
		return imgs;
	}

	cv::Mat_<double>  gaussian_filter( cv::Mat_<double> &img, int sigma) {
		int cols = img.cols;
		int rows = img.rows;
		cv::Mat_<double> mat_gauss(rows, cols);

		int N = sigma*3;
		double fx, fy;
		double gauss, gauss_val;
		int x_1, x_2, y_1, y_2;

		for (int y=0; y<rows; y++) {
			for (int x=0; x<cols; x++) {
				gauss = 0;
				for (int i=-N; i<=N; i++) {
					for (int j=-N; j<=N; j++) {
						gauss_val = gaussian(sigma, sqrt(i*i + j*j)); ///// gaussian(sigma, x);
						gauss += gauss_val*get_img_val(y+i, x+j, img);
						// std::cout << gauss_val << ' ' << img(i, j) << std::endl;
					}
				}
				mat_gauss(y, x) = gauss;
			}
		}
		return mat_gauss;
	}
	double gaussian(int sigma, double x) {
		double gauss = (1.0/(sigma*sigma*2.0*M_PI))*exp(-x*x/(2.0*sigma*sigma) ) ;
		return gauss;
	}
	cv::Mat_<double> response(cv::Mat_<double> g_fx2, cv::Mat_<double> g_fy2, cv::Mat_<double> g_fxy) {
		int rows = g_fx2.rows, cols = g_fx2.cols;
		double detC, trC, res;
		double k = 0.08 ;
		cv::Mat_<double> mat_res(rows, cols);

		for (int y=0; y<rows; y++) {
			for (int x=0; x<cols; x++) {
				detC = g_fx2(y, x)*g_fy2(y, x) - 2*g_fxy(y, x);
				trC = g_fx2(y, x) + g_fy2(y, x);
				res = detC - k*trC*trC;
				// std::cout << "res = " << res << std::endl;
				mat_res(y, x) = res;
			}
		}
		// cv::imwrite(this->img_dir + "res.png" , mat_res);
		return mat_res;
	}

	cv::Mat_<unsigned char> maximum(cv::Mat_<double> res) {
		double rows = res.rows, cols = res.cols;
		cv::Mat_<unsigned char> maximum(rows, cols);
		for (int y=0; y<rows; y++) {
			for (int x=0; x<cols; x++) {
				if (is_maximum(y, x, res) == true) {
					maximum(y, x) = UCHAR_MAX;
				} else {
					maximum(y, x) = 0;
				}
			}
		}
		// cv::imwrite(this->img_dir + "" + filename, maximum);
		return maximum;
	}

	cv::Mat_<unsigned char> mark_corner(cv::Mat_<unsigned char> src, cv::Mat_<unsigned char>  corner) {
		double rows = src.rows, cols = src.cols;
		for (int y=0; y<rows; y++) {
			for (int x=0; x<cols; x++) {
				if  (corner(y, x) == 255) {
					cv::Point2d p(y, x) ;
					mark(src, p, 140);
				}
			}
		}
		// cv::imwrite(this->img_dir + "marked_chessboard.png", src);
		return src;
	}
	///// return LoG matrix 
	///// args: src image, feature points image
	cv::Mat_<int> get_log_mat(cv::Mat_<unsigned char> src, cv::Mat_<unsigned char> feature, double sigma) {
		int rows = src.rows, cols = src.cols;
		cv::Mat_<int> log_mat(rows, cols);
		for (int y=0; y<rows; y++) {
			for (int x=0; x<cols; x++) {
				if (feature[y][x] == UCHAR_MAX) {
					// std::string str_y = std::to_string(y);
					// std::string str_x = std::to_string(x);
					std::stringstream str_yx;
					str_yx << '[' << y << "][" << x << "]"<< std::endl;
					// comment_timestamp("feature"+str_yx.str()+"="+std::to_string(feature[y][x]));
					log_mat[y][x] = get_log_val(src, y, x, sigma);
					// std::cout << "log_mat" << str_yx << "=" << log_mat[y][x] << std::endl;
				}
			}
		}
		// std::cout << log_mat << std::endl;
		return log_mat;
	}

	void comment_timestamp(std::string comment) {
		time_t now = time(0);
		// char* dt = ctime(&now);
		tm *ltm = localtime(&now);
		std::cout << ltm->tm_hour << ':' << ltm->tm_min << ' ' << ltm->tm_sec << "\t" << comment << std::endl;
	}

private:
		///// return LoG = sigma^2(Bxx + Byy)
		int get_log_val(cv::Mat_<unsigned char> src, int y, int x, double sigma) {
			double fx[3], fy[3];
			for (int i=-1; i<=1; i++) {
				// fx[i] = (src[y][x+i-1] - src[y][x+i+1]) / 2;
				// fy[i] = (src[y+i-1][x] - src[y+i-1][x]) / 2;
				fx[i] = (get_img_val(y, x+i-1, src) - get_img_val(y, x+i+1, src)) / 2;
				fy[i] = (get_img_val(y+i-1, x, src) - get_img_val(y+i+1, x, src)) / 2;
			}
			double fxx, fyy;
			fxx = (fx[0] - fx[2]) / 2;
			fyy = (fy[0] - fy[2]) / 2;
			int log_val = sigma*sigma*abs(fxx + fyy);
			return log_val;
		}

		void mark(cv::Mat& img, cv::Point2d &p, unsigned char l) {
		int r = 10;
		double x = p.x;
		double y = p.y;
		cv::Point2d pt(x,y);
		cv::Point2d pt1(x - r / 2, y - r / 2);
		cv::Point2d pt2(x + r / 2, y + r / 2);
		cv::Point2d pt3(x - r / 2, y + r / 2);
		cv::Point2d pt4(x + r / 2, y - r / 2);

		// std::cout << p << std::endl;
		if (x >= 0 && y >= 0 && x <= img.rows && y <= img.cols) {
			cv::circle(img, pt, 10, l);
			cv::line(img, pt1, pt2, l);
			cv::line(img, pt3, pt4, l);
		}
	}
	bool is_maximum(int y, int x, cv::Mat_<double> img) {
		double maximum = get_img_val(y, x, img);
		double tmp = 0;
		for (int i = -1; i <= 1; i++) {
			for (int j=-1; j<=1; j++) {
				tmp = get_img_val(y+i, x+j, img);
				if (i==0 && j==0) continue;
				if (tmp >= maximum) {
					return false;
				}				
			}
		}
		return true;
	}
	double get_img_val(int y, int x, cv::Mat_<double> img) {
		int cols = img.cols;	
		int rows = img.rows;
		if (x < 0) x= -x-1;
		if (x >= cols) x =  (cols-1) - (x - (cols-1));
		if (y < 0) y= -y-1;
		if (y >= rows) y =  (rows-1) - (y - (rows-1)) ;
		double val = img(y, x);
		return val;
	}
};


bool is_muximum(int maximum, int val1, int val2) {
	if (val1 > maximum) return false;
	if (val2 > maximum) return false;
	return true;
}

int main() {
	Harris harris = Harris();

	harris.img_dir = "img2/";
	harris.ext = ".png";
	harris.sig_str = "";
	double sigma = 1;
	double k = 1.2;
	double N = 8;
	cv::Mat_<double> img = cv::imread(harris.img_dir + "Chessboard" + harris.ext, 0);
	int rows = img.rows, cols = img.cols;
	std::vector<std::string> filenames;
	// for (int i=0; i< N; i++) {
	// 	// harris.sig_str = std::to_string(sigma);
	// 	std::stringstream ss;
	// 	ss << sigma;
	// 	harris.sig_str = ss.str();

	// 	cv::Mat_<double> scaled(img.rows, img.cols);
	// 	scaled = harris.gaussian_filter(img, sigma);
	// 	harris.comment_timestamp("gaussian end");
	// 	cv::imwrite(harris.img_dir + "gaussian"+harris.sig_str+harris.ext, scaled);
	// 	// cv::Mat_<double> mat_LoG(img.rows, img.cols);
	// 	cv::Mat_<unsigned char> corner = harris.harris_operator(scaled, sigma);
	// 	harris.comment_timestamp("harris operator end");
	// 	// log_imgs.push_back(corner);
	// 	cv::Mat_<unsigned char> src = cv::imread(harris.img_dir + "gaussian"+harris.sig_str+harris.ext, 0);
	// 	cv::Mat_<unsigned char> feature = cv::imread(harris.img_dir + "feature_points"+harris.sig_str+".png", 0);
	// 	cv::Mat_<double> log_mat = harris.get_log_mat(src, feature, sigma);
	// 	harris.comment_timestamp("LoG end");
	// 	cv::imwrite(harris.img_dir+"LoG"+harris.sig_str+harris.ext, log_mat);
	// 	sigma += k;
	// 	// harris.comment_timestamp(std::to_string(i) + "th turn end");
	// }
	
	///// make Log images
	std::vector<cv::Mat_<int> > log_imgs;
	sigma = 1;
	for (int i=0; i<N; i++) {
		// harris.sig_str = std::to_string(sigma);
		std::stringstream ss;
		ss << sigma;
		harris.sig_str = ss.str();

		cv::Mat_<unsigned char> feature = cv::imread(harris.img_dir + "feature_points" +harris.sig_str+ harris.ext, 0);
		cv::Mat_<unsigned char> src 	= cv::imread(harris.img_dir + "gaussian"+harris.sig_str	+ harris.ext, 0);
		std::cout << harris.img_dir + "feature_points" +harris.sig_str+ harris.ext << std::endl;
		std::cout << harris.img_dir + "gaussian"+harris.sig_str + harris.ext << std::endl;
		std::cout << "feature=" << feature.size() << "\tsrc=" << src.size() << std::endl;
		cv::Mat_<int> log_mat = harris.get_log_mat(src, feature, sigma);
		cv::imwrite(harris.img_dir+"LoG"+harris.sig_str+harris.ext, log_mat);
		log_imgs.push_back(log_mat);
		sigma += k;
	}

	///// decide LoG maximum
	// std::vector<cv::Mat_<unsigned char> > harris_laplacian1s; //// (y, x, val);
	// std::vector<double[3]> feature_points;
	int threshold = 10;
	sigma = 1;
	for (int i=0; i<N; i++) {	
		cv::Mat_<unsigned char> harris_laplacian1(rows, cols);	
		std::stringstream ss;
		ss << sigma;
		harris.sig_str = ss.str();
		for (int y=0; y<rows; y++) {
			for (int x=0; x<rows; x++) {
				int  max = 0;
					if (int val=log_imgs[i](y, x) == threshold) {
						max = val;
						int val1 = log_imgs[i-1](y, x) ;
						int val2 = log_imgs[i+1](y, x);
						if (is_muximum(max, val1, val2)) {
							harris_laplacian1(y, x) = UCHAR_MAX;
						}
					}
			}
		}
		// std::cout  << harris_laplacian1 << std::endl;

		imwrite(harris.img_dir + "harris_laplacian1_" + harris.sig_str + harris.ext, harris_laplacian1);
		sigma += k;
	}
	return 0;
}
