#include <opencv2/opencv.hpp>
#include <sstream>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <opencv2/highgui/highgui.hpp>

#define N 6

int main() {
	std::vector<cv::Point3d>x_0;
	std::vector<cv::Point3d>x_1;
	std::vector<cv::Point3d>x_2 ;
	double x, y, z; 
	std::ifstream  file1("world_position1.txt");
	for (int i=0; i<N; i++) {
		file1 >> x >> y >> z;
		cv::Point3d  x0i(x, y, z); 
		x_0.push_back(x0i);
	}
	std::ifstream file2("room1.txt");
	for (int i=0; i<N; i++) {
		file2 >> x >> y >> z;
		cv::Point3d  x1i(x, y, z); 
		x_1.push_back(x1i);
	}
	std::ifstream file3( "room2.txt");
	for (int i=0; i<N; i++) {
		file3 >> x >> y >> z;
		cv::Point3d  x2i(x, y, z); 
		x_2.push_back(x2i);
	}
	//  Camera1  focal length: 18mm,  sensor size:  23.5 x 15.7mm,
	Camera c1;
	c1.set_focal(18);
	c1.set_ccd(23);

	// std::cout << x_1 << std::endl;
	cv::Mat A;
	for (int i=0; i<(int)x_0.size(); i++) {
		int x1 = x_1[i].x, y1 = x_1[i].y, w1 = x_1[i].z;	
		int x0 = x_0[i].x, y0 = x_0[i].y, z0 = x_0[i].z;

		// std::cout << x_1[i] << std::endl;
		cv::Mat  Ai = (cv::Mat_<double> (2,9)  << 0,0,0,  -w1*x0, -w1*y0, -w1*z0,   y1*x0, y1*y0, y1*z0,
					w1*x0, w1*y0, w1*z0, 0, 0, 0,  -x1*x0, -x1*y0, -x1*z0);
		// A <<  0,0,0,  -w1*x0, -w1*y0, -w1*z0,   y1*x0, y1*y0, y1*z0,  w1*x0, w1*y0, w1*z0, 0, 0, 0,  -x1*x0, -x1*y0, -x1*z0 ;
		A.push_back(Ai);
	}
	cv::Mat w, u, vt;  // vt(9, 9)
	cv::SVD::compute(A, w, u, vt, cv::SVD::FULL_UV);
	// std::cout << vt <<  std::endl;
	//   h1, h2, h3 =  [vt00, vt10, vt20, ... , vt80]
	//   H = [h1, h2, h3]
	cv::Mat H = (cv::Mat_<double> (3, 3)  <<  vt.at<double>(0,0), vt.at<double>(1,0), vt.at<double>(2,0), vt.at<double>(3,0), vt.at<double>(4,0), 
		vt.at<double>(5,0), vt.at<double>(6,0), vt.at<double>(7,0), vt.at<double>(8,0) );
	 std::cout << H << std::endl;

	return 0;
}
