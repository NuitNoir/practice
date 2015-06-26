#include <opencv2/opencv.hpp>
#include <sstream>
#include <fstream>
#include <iostream>
#include <istream>
// #include <string>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include "../lib/camera.hpp"
#include "../lib/affin.hpp"
#include "../lib/transform_3d.hpp"
#define N  8

cv::Mat_<double> homo_graphy(std::vector<cv::Point3d>  x_1,  std::vector<cv::Point3d>  x_2 ) ;
std::vector<cv::Point3d> read_positions(char*  file_name) ;
cv::Matx33d get_fundamental_matrix(std::vector<cv::Point3d> x1 , std::vector<cv::Point3d> x2) ;
// cv::Mat_<double> get_fundamental_matrix(std::vector<cv::Point3d> x1 , std::vector<cv::Point3d> x2) ;
void draw_epiline(cv::Mat &img, const Point3d &u1, const Matx33d &F) ;
// void draw_epiline(cv::Mat &img, const PPoint2d &u1, const Mat_<double> &F) ;
void draw_line(cv::Mat &img, const PPoint2d &p, unsigned char l) ; 

tutor::PPoint2d pt_to_ppt(Point3d pt) ;
cv::Mat_<double> ppt_to_mat(tutor::PPoint2d ppt) ;

int main() {
	std::vector<cv::Point3d>u1;
	std::vector<cv::Point3d>u2 ;

	char* file_names[] = {"imgs/img12.txt", "imgs/img22.txt"};
	char* img_names[] = {"imgs/image1.pgm", "imgs/image2.pgm"};
	///// get points
	u1 = read_positions(file_names[0]);
	u2 = read_positions(file_names[1]);
	std::cout << "u1=" << u1 << std::endl;
	std::cout << "u2=" << u2 << std::endl;
	///// get images
	cv::Mat img1 = cv::imread(img_names[0]);
	cv::Mat img2 = cv::imread(img_names[1]);

	///// get F
	cv::Matx33d F ;
	// cv::Mat_<double> F;
	F = get_fundamental_matrix(u1, u2);
	// std::cout << "F=" << F << std::endl;
	///// draw epipoler line
	for (unsigned i=0; i<u1.size(); i++) {
		PPoint2d u = pt_to_ppt(u1[i]);
		draw_epiline(img1, u, F) ;
	}
	for (unsigned i=0; i<u2.size(); i++) {
		PPoint2d u = pt_to_ppt(u2[i]);
		draw_epiline(img2, u, F.t()) ;
	}
	cv::imwrite("image1.jpg", img1);
	cv::imwrite("image2.jpg", img2);
	return 0;
}

void draw_epiline(cv::Mat &img, const Point3d &u1, const Matx33d &F) {
// void draw_epiline(cv::Mat &img, PPoint2d &u1, Mat_<double> &F) {
	PPoint2d l = F*u1;
	std::cout << "u1=" << u1 << std::endl;
	// std::cout << "F=" << F << std::endl;
	std::cout << "l=" << l<< std::endl;
	draw_line(img, l, 1);
	 // cv::imwrite("img.jpg", img);

}

// tutor::PPoint2d ppt_matx(PPint2d ppt, Mat_<double> mat) {
// 	tutor::PPoint2d pt(ppt[0]*mat(0,0) + ppt[1]*mat(0,1) + ppt[2]*mat(0,2),
// 		ppt[0]*mat(1,0) + ppt[1]*mat(1,1) + ppt[2]*mat(1,2),
// 		ppt[0]*mat(2,0) + ppt[1]*mat(2,1) + ppt[2]*mat(2,2),
// 	  ) ;
// }

cv::Matx33d get_fundamental_matrix(std::vector<cv::Point3d> x1 , std::vector<cv::Point3d> x2) {
	cv::Mat_<double> W;
	cv::Mat_<double> u, d, vt;

	for (int i=0; i< (int)x1.size(); i++ ) {
		cv::Mat_<double> Wi = (cv::Mat_<double>(1, 9)  <<  x1[i].x*x2[i].x, x1[i].y*x2[i].x, x2[i].x, 
			x1[i].x*x2[i].y, x1[i].y*x2[i].y, x2[i].y, x1[i].x, x1[i].y, 1);
		W.push_back(Wi);
	}
	cv::SVD::compute(W, d, u, vt, cv::SVD::FULL_UV);
	// std::cout << "d=" << d << endl;
	// std::cout << "U=" <<  u << endl;
	// std::cout << "vt=" << vt << endl;
	cv::Mat_<double> F = (cv::Mat_<double>(3,3)   <<  vt(8,0),vt(8,1),vt(8,2),vt(8,3),vt(8,4),vt(8,5),vt(8,6),vt(8,7),vt(8,8) ) ;
	// cv::Matx33d F(vt(8,0),vt(8,1),vt(8,2),vt(8,3),vt(8,4),vt(8,5),vt(8,6),vt(8,7),vt(8,8) ) ;
	// std::cout << "F=" << F << std::endl;
	//// decompose F 
	//// Now, F's degree of freedom is 3, but actually F's degree of freedom is 2
	///// so reduce F's degree of freedom
	cv::SVD::compute(F, d, u, vt, cv::SVD::FULL_UV);
	// std::cout << "d=" << d << std::endl;
	cv::Mat_<double> D   = (cv::Mat_<double> (3,3)  <<  d(0), 0, 0,    0, d(1), 0,    0, 0, 0);
	// std::cout << "Reduced D=" << D << std::endl;
	F  = u*D*vt;
	// std::cout << "Reduced F=" << F << std::endl;	
	return F;
}

std::vector<cv::Point3d> read_positions(char*  file_name) {
	std::vector<cv::Point3d> x_2;
	double x, y;

	std::ifstream file(file_name);
	for (int i=0; i<N; i++) {
		file >> x >> y;
		cv::Point3d  x2i(x, y, 1); 
		x_2.push_back(x2i);
	}
	return x_2;
}



tutor::PPoint2d pt_to_ppt(Point3d pt) {
	tutor::PPoint2d  ppt(pt.x, pt.y, pt.z);
	return ppt;
}
cv::Mat_<double> ppt_to_mat(tutor::PPoint2d ppt) {
	cv::Mat_<double> mat = (cv::Mat_<double>(1,3)  <<  ppt[0], ppt[1], ppt[2]);
	return mat;
}


void draw_line(cv::Mat &img, const PPoint2d &p, unsigned char l) {

	// P = (a, b, c)
	//	a*x + b*y + c = 0
	//		y = -a/b*x - c/b
	//		y = -p[0]/p[1]*x - p[2]/p[1]
	//
	//		x = -b/a*y - c/a
	//		x = -p[1]/p[0]*y - p[2]/p[0]
	  double slope;
	  double intercept;
	  int height = img.rows;
	  int width = img.cols;
	  int MAX_VAL = 1000000000;
	  slope = p[0]/p[1];


	  // SLOPE SIZE < abs(1)
	  if (abs(slope) <= 1) {
	    int x = 0;
	    slope = -p[0]/p[1];
	    intercept = p[2]/p[1];
	    cout << slope << ' ' << intercept << endl;
	    double y = slope*x - intercept;
	    //slope < 1 to right
	    for (int x=0; x<width; x++) {
	      //			std::cout << slope << std::endl;
	      //			std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;
	      //y += slope;
	      y = slope*x - intercept;
	    	if (y < 0 || y > height) continue;
	      img.at<unsigned char>(static_cast<int>(y + 0.5), x) = l;
	    }
	  } else {
	    // when slope > abs(1)
	    slope = -p[1]/p[0];
	    intercept = p[2]/p[0];
	    cout << slope << ' ' << intercept << endl;
	    int y = 0;
	    double x = slope*y - intercept;
	    // slope > 1 to right
	    for (int y=0; y<height; y++) {
	      //			std::cout << slope << std::endl;
	      //			std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;
	      //x += slope;
	      x = slope*y - intercept;
	    	if (x > width || x < 0) continue;
	      img.at<unsigned char>(y, static_cast<int>(x + 0.5)) = l;
	    }
	  }
	  // cv::imwrite("img.jpg", img);
}
