#include <opencv2/opencv.hpp>
#include <sstream>
#include <fstream>
#include <iostream>
#include <istream>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include "../lib/camera.hpp"
#include "../lib/drawLine.hpp"
#include "../lib/affin.hpp"
#include "../lib/transform_3d.hpp"
#define N  8

cv::Mat_<double> homo_graphy(std::vector<cv::Point3d>  x_1,  std::vector<cv::Point3d>  x_2 ) ;
std::vector<tutor::PPoint2d> read_positions(char*  file_name) ;
cv::Matx33d get_fundamental_matrix(std::vector<tutor::PPoint2d> x1 , std::vector<tutor::PPoint2d> x2) ;
// cv::Mat_<double> get_fundamental_matrix(std::vector<cv::Point3d> x1 , std::vector<cv::Point3d> x2) ;
void draw_epiline(cv::Mat &img, const Point3d &u1, const Matx33d &F) ;
// void draw_epiline(cv::Mat &img, const PPoint2d &u1, const Mat_<double> &F) ; 

tutor::PPoint2d pt_to_ppt(Point3d pt) ;
cv::Mat_<double> ppt_to_mat(tutor::PPoint2d ppt) ;

int main() {
	std::vector<tutor::PPoint2d>u1;
	std::vector<tutor::PPoint2d>u2 ;

	char* file_names[] = {"imgs/img12.txt", "imgs/img22.txt"};
	char* img_names[] = {"imgs/image1.pgm", "imgs/image2.pgm"};
	///// get points
	u1 = read_positions(file_names[0]);
	u2 = read_positions(file_names[1]);
	// std::cout << "u1=" << u1 << std::endl;
	// std::cout << "u2=" << u2 << std::endl;
	///// get images
	cv::Mat img1 = cv::imread(img_names[0]);
	cv::Mat img2 = cv::imread(img_names[1]);

	Drawer drawer = Drawer();
	for (unsigned  i=0; i<N; i++) {
		drawer.mark(img1, u1[i], 255);
		drawer.mark(img2, u2[i],  255);
	}
	///// get F
	cv::Matx33d F ;
	F = get_fundamental_matrix(u1, u2);
	///// draw epipoler line
	for (unsigned i=0; i<u1.size(); i++) {
		// PPoint2d u = pt_to_ppt(u1[i]);
		std::cout << "i=" << i  << "   " ;
		draw_epiline(img1, u1[i], F) ;
	}
	for (unsigned i=0; i<u2.size(); i++) {
		// PPoint2d u = pt_to_ppt(u2[i]);
		draw_epiline(img2, u2[i], F.t()) ;
	}
	cv::imwrite("image1.jpg", img1);
	cv::imwrite("image2.jpg", img2);
	return 0;
}

void draw_line_p(cv::Mat &img, const tutor::PPoint2d &p) {
	// P = (a, b, c)
	//	a*x + b*y + c = 0
	//		y = -a/b*x - c/b
	//		y = -p[0]/p[1]*x - p[2]/p[1]
	double slope;
	double intercept;
	int height = img.rows;
	int width = img.cols;
	int MAX_VAL = 1000000000;
	slope = p[0]/p[1];
	intercept = -p[2]/p[1];
	std::cout << "slope=" << slope << "\tintercept=" << intercept << std::endl;
	double x, y;

	/////  x = 0
	x = 0;
	y =  intercept ;
	cv::Point2d pt1(x, y);
	//// x = width
	x = width;
	y = slope*x + intercept;
	cv::Point2d pt2(x, y);
	cv::line(img, pt1, pt2, 255);
}

void draw_epiline(cv::Mat &img, const Point3d &u1, const Matx33d &F) {
// void draw_epiline(cv::Mat &img, PPoint2d &u1, Mat_<double> &F) {
	cv::Point3d l = F*u1;
	tutor::PPoint2d pt(l.x, l.y, l.z);
	std::cout << "u1=" << u1 << std::endl;
	// std::cout << "F=" << F << std::endl;
	std::cout << "l=" << l<< std::endl;
	std::cout << "pt=" << pt << std::endl;

	draw_line_p(img, pt);
	// Drawer drawer = Drawer();
	// drawer.draw_line_p(img, l, 1);
	 // cv::imwrite("img.jpg", img);

}

cv::Matx33d get_fundamental_matrix(std::vector<tutor::PPoint2d> x1 , std::vector<tutor::PPoint2d> x2) {
	cv::Mat_<double> W;
	cv::Mat_<double> u, d, vt;

	for (int i=0; i< (int)x1.size(); i++ ) {
		cv::Mat_<double> Wi = (cv::Mat_<double>(1, 9)  <<  x1[i].x()*x2[i].x(), x1[i].y()*x2[i].x(), x2[i].x(), 
			x1[i].x()*x2[i].y(), x1[i].y()*x2[i].y(), x2[i].y(), x1[i].x(), x1[i].y(), 1);
		W.push_back(Wi);
	}
	cv::SVD::compute(W, d, u, vt, cv::SVD::FULL_UV);
	// std::cout << "d=" << d << endl;
	// std::cout << "U=" <<  u << endl;
	// std::cout << "vt=" << vt << endl;
	cv::Mat_<double> F = (cv::Mat_<double>(3,3)   <<  vt(8,0),vt(8,1),vt(8,2),vt(8,3),vt(8,4),vt(8,5),vt(8,6),vt(8,7),vt(8,8) ) ;
	// cv::Matx33d F(vt(8,0),vt(8,1),vt(8,2),vt(8,3),vt(8,4),vt(8,5),vt(8,6),vt(8,7),vt(8,8) ) ;

	//// decompose F 
	//// Now, F's degree of freedom is 3, but actually F's degree of freedom is 2.  so reduce F's degree of freedom
	cv::SVD::compute(F, d, u, vt, cv::SVD::FULL_UV);
	// std::cout << "d=" << d << std::endl;
	cv::Mat_<double> D   = (cv::Mat_<double> (3,3)  <<  d(0), 0, 0,    0, d(1), 0,    0, 0, 0);
	// std::cout << "Reduced D=" << D << std::endl;
	F  = u*D*vt;
	F = F / F(2,2);
	std::cout << "F=" << F << std::endl;

	// std::cout << "Reduced F=" << F << std::endl;	
	return F;
}

std::vector<tutor::PPoint2d> read_positions(char*  file_name) {
	std::vector<tutor::PPoint2d> x_2;
	double x, y;

	std::ifstream file(file_name);
	for (int i=0; i<N; i++) {
		file >> x >> y;
		tutor::PPoint2d  x2i(x, y, 1); 
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

