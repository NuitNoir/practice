#include <opencv2/opencv.hpp>
#include <sstream>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include "camera.hpp"
#include "transform_3d.hpp"
#include "affin.hpp"
#define N 4

int main() {
	std::vector<cv::Point3d>x_1;
	std::vector<cv::Point3d>x_2 ;
	double x, y, z; 

	std::ifstream file2("chess1.txt");
	for (int i=0; i<N; i++) {
		file2 >> x >> y >> z;
		cv::Point3d  x1i(x, y, z); 
		x_1.push_back(x1i);
	}
	std::cout << x_1 << endl;
	std::ifstream file3( "chess2.txt");
	for (int i=0; i<N; i++) {
		file3 >> x >> y >> z;
		cv::Point3d  x2i(x, y, z); 
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
		int x1 = x_1[i].x, y1 = x_1[i].y, w1 = x_1[i].z;
		int x2 = x_2[i].x, y2 = x_2[i].y, z2 = x_2[i].z;
		cv::Mat  Ai = (cv::Mat_<double> (2,9)  << 0, 0,0,  -w1*x2, -w1*y2, -w1*z2,   y1*x2, y1*y2, y1*z2,
					w1*x2, w1*y2, w1*z2, 0, 0, 0,  -x1*x2, -x1*y2, -x1*z2);
		A.push_back(Ai);
	}
	cv::Mat w, u, vt;  // vt(9, 9)
	cv::SVD::compute(A, w, u, vt, cv::SVD::FULL_UV);
	////////////////////   h1, h2, h3 =  [vt00, vt10, vt20, ... , vt80]
	////////////////////   H = [h1, h2, h3]
	std::cout << vt << endl;
	cv::Mat H = (cv::Mat_<double> (3, 3)  <<  vt.at<double>(8,0) , vt.at<double>(8,1) , vt.at<double>(8,2) , vt.at<double>(8,3) , vt.at<double>(8,4) , 
		vt.at<double>(8,5) , vt.at<double>(8,6) , vt.at<double>(8,7) , vt.at<double>(8,8)  );
	 std::cout << H << std::endl;
	 // //////////////////////// image transform
	 cv::Mat img = cv::imread("chess1.pgm", 1);
	 AffinTransformer affin;
	 // cv::Mat out_img = affin.TransformE(img, H, 0);
	 cv::Mat out_img = affin.TransformL(img, H);
	 cv::imwrite("out.jpg", out_img);

	return 0;
}


