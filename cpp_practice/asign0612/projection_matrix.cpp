

#include <opencv2/opencv.hpp>
#include <sstream>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include "camera.hpp"
#include "affin.hpp"
#include "ppoint.hpp"
#define N  6

int main() {
	std::vector<tutor::PPoint3d>x_0;
	std::vector<cv::Point3d>x_1;
	std::vector<cv::Point3d>x_2 ;
	double x, y, z;

	std::ifstream file0("world_position1.txt");
	for (int i=0; i<N; i++) {
		file0  >> x >> y >> z;
		tutor::PPoint3d x0i(x, y, z, 1);
		x_0.push_back(x0i);
	}

	std::ifstream file2("pts1.txt");
	// std::ifstream file2("chess1.txt");
	for (int i=0; i<N; i++) {
		file2 >> x >> y;
		cv::Point3d  x1i(x, y, 1);
		x_1.push_back(x1i);
	}
	std::cout << x_1 << endl;
	std::ifstream file3( "pts2.txt");
	std::ifstream file3( "chess2.txt");
	for (int i=0; i<N; i++) {
		file3 >> x >> y;
		cv::Point3d  x2i(x, y, 1); 
		x_2.push_back(x2i);
	}
	std::cout << x_2 << endl;

	//////////  Camera1  focal length: 18mm,  sensor size:  23.5 x 15.7mm,
	Camera c1;
	c1.set_focal(18);
	c1.set_ccd(23);
	Camera c2;
	c2.set_focal(18);
	c2.set_ccd(23);

	cv::Mat A;
	for (int i=0; i<(int)x_1.size(); i++) {
		// std::cout << "i=" << i << ",  x_1=" << x_1[i] << ",  x_2=" << x_2 [i]<< std::endl;
		int x0 = x_0[i][0], y0 = x_0[i][1], z0 = x_0[i][2], w0=x_0[i][3];
		// int x1 = x_1[i].x, y1 = x_1[i].y, w1 = x_1[i].z;
		int x1 = x_2[i].x, y1 = x_2[i].y, w1 = x_2[i].z;
		cv::Mat  Ai = (cv::Mat_<double> (2,12)  << 0,0,0,0,  -w1*x0, -w1*y0, -w1*z0, -w1*w0,   y1*x0, y1*y0, y1*z0, y1*w0,
					w1*x0, w1*y0, w1*z0, w1*w0,  0,0,0,0,  -x1*x0, -x1*y0, -x1*z0, -x1*w0);
		A.push_back(Ai);
	}
	std::cout << "A" << std::endl << A << std::endl;
	cv::Mat w, u, vt;  // vt(12, 12)
	cv::SVD::compute(A, w, u, vt, cv::SVD::FULL_UV);
	////////////////////   h1, h2, h3 =  [vt00, vt10, vt20, ... , vt80]
	////////////////////   H = [h1, h2, h3]
	std::cout << "w=" <<  w << endl; // 12
	std::cout << "u=" << u << endl; // 12*12
	std::cout << "vt=" << vt << endl; // 12*12
	cv::Mat P = (cv::Mat_<double> (3, 4)  <<  vt.at<double>(11,0) , vt.at<double>(11,1) , vt.at<double>(11,2) , vt.at<double>(11,3) , vt.at<double>(11,4) , 
		vt.at<double>(11,5) , vt.at<double>(11,6) , vt.at<double>(11,7) , vt.at<double>(11,8), vt.at<double>(11,9), vt.at<double>(11,10), vt.at<double>(11,11) );
	cv::Mat P_ = (cv::Mat_<double> (3, 4)  <<  vt.at<double>(11,0) , vt.at<double>(11,1) , vt.at<double>(11,2) ,  vt.at<double>(11,4) , 
		vt.at<double>(11,5) , vt.at<double>(11,6) ,  vt.at<double>(11,8), vt.at<double>(11,9), vt.at<double>(11,10) );
	 std::cout << "## projection matrix" <<  std::endl << P << std::endl;

	 ///////////////////// intrinsic parameter
	 cv::Point3d p3(vt.at<double>(11,8), vt.at<double>(11,9), vt.at<double>(11,10) );
	 double f = sqrt(p3.x*p3.x + p3.y*p3.y + p3.z*p3.z) ;
	 std::cout << "f=" << f<< std::endl;


	return 0;
}


