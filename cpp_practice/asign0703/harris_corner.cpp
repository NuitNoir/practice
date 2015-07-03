#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../lib/ppoint.hpp"
// #include "../lib/drawLine.hpp"

void differentiate(cv::Mat_<int> &img) ;

class Harris {
public:
	cv::Mat_<double> harris_operator(cv::Mat_<double> img, double sigma) {
		// cv::Mat_<double> img = cv::imread("Chessboard.png", 0);

		std::vector<cv::Mat_<double> > imgs;
		imgs = differentiate(img);
		///// gauss 
		cv::Mat_<double> fx2 = cv::imread("img/fx2.png", 0);
		cv::Mat_<double> fy2 = cv::imread("img/fy2.png", 0);
		cv::Mat_<double> fxy = cv::imread("img/fxy.png", 0);
		gaussian_filter(fx2, sigma, "gauss_fx2.png");
		gaussian_filter(fy2, sigma, "gauss_fy2.png");
		gaussian_filter(fxy, sigma, "gauss_fxy.png");

		///// response
		cv::Mat_<double> g_fx2 = cv::imread("img/gauss_fx2.png", 0);		
		cv::Mat_<double> g_fy2 = cv::imread("img/gauss_fy2.png", 0);		
		cv::Mat_<double> g_fxy = cv::imread("img/gauss_fxy.png", 0);		
		cv::Mat_<double> res = response(g_fx2, g_fy2, g_fxy);

		// maximum
		// cv::Mat_<double> res = cv::imread("img/res.png", 0);
		
		cv::Mat_<unsigned char> corner = maximum(res);
		// mark src img
		cv::Mat_<unsigned char> src = cv::imread("img/Chessboard.png", 2);
		// cv::Mat_<unsigned char> corner = cv::imread("img/maximum.png", 0);
		cv::Mat_<unsigned char> marked_src = mark_corner(src, corner);
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
		cv::imwrite("img/fx2.png", mat_fx2);
		cv::imwrite("img/fy2.png", mat_fy2);
		cv::imwrite("img/fxy.png", mat_fxy);
		return imgs;
	}

	cv::Mat_<double>  gaussian_filter( cv::Mat_<double> &img, int sigma, std::string filename) {
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
		// std::cout << mat_fx << std::endl;
		cv::imwrite("img/" + filename, mat_gauss); 
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
				mat_res(y, x) = res;
			}
		}
		cv::imwrite("img/res.png" , mat_res);
		return mat_res;
	}

	cv::Mat_<unsigned char> maximum(cv::Mat_<double> res) {
		double rows = res.rows, cols = res.cols;
		cv::Mat_<unsigned char> maximum(rows, cols);
		for (int y=0; y<rows; y++) {
			for (int x=0; x<cols; x++) {
				if (is_maximum(y, x, res) == true) {
					maximum(y, x) = 255;
				} else {
					maximum(y, x) = 0;
				}
			}
		}
		cv::imwrite("img/maximum.png", maximum);
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
		cv::imwrite("img/marked_chessboard.png", src);
		return src;
	}
			
private:
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

int main() {
	Harris harris = Harris();

	double sigma = 1;
	double k = 1.2;
	double N = 10;
	cv::Mat_<double> img = cv::imread("Chessboard.png", 0);
	std::vector<cv::Mat_<double> > log_imgs;
	// std::vector<std::string> filenames  

	for (int i=0; i< N; i++) {
		cv::Mat_<double> scaled(img.rows, img.cols);
		scaled = harris.gaussian_filter(img, sigma, "gaussian.png");
		cv::Mat_<double> mat_LoG(img. rows, img.cols);
		corner = harris.harris_operator(scaled, sigma);
		log_imgs.push_back(corner);
		sigma += k;
		std::cout << i << std::endl;
	}

	double rows = img.rows, cols = img.cols;
	for (unsigned i=0; i<log_imgs.size(); i++) {
		// log_imgs[i] 
	}
	// harris.harris_operator(scaled);
	return 0;
}

