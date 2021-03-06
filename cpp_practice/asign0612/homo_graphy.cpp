#include <opencv2/opencv.hpp>
#include <sstream>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include "../lib/camera.hpp"
#include "../lib/affin.hpp"
#include "../lib/transform_3d.hpp"
#define N  5

int main() {
	std::vector<cv::Point3d>x_1;
	std::vector<cv::Point3d>x_2;
	double x, y;

	std::ifstream file2("chess1.txt");
	// std::ifstream file2("chess1.txt");
	for (int i=0; i<N; i++) {
		file2 >> x >> y;
		cv::Point3d  x1i(x, y, 1);
		x_1.push_back(x1i);
	}
	std::cout << "x_1 = " <<   x_1 << endl;
	std::ifstream file3( "chess2.txt");
	// std::ifstream file3( "chess2.txt");
	for (int i=0; i<N; i++) {
		file3 >> x >> y;
		cv::Point3d  x2i(x, y, 1); 
		x_2.push_back(x2i);
	}
	std::cout << "x_2 = " <<  x_2 << endl;

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
		int x1 = x_1[i].x, y1 = x_1[i].y, w1 = x_1[i].z;
		int x2 = x_2[i].x, y2 = x_2[i].y, z2 = x_2[i].z;
		// Ai = (0   -w*X y*X
		//		 w*X 0    -x*X)
		cv::Mat  Ai = (cv::Mat_<double> (2,9)  << 0, 0,0,  -w1*x2, -w1*y2, -w1*z2,   y1*x2, y1*y2, y1*z2,
					w1*x2, w1*y2, w1*z2, 0, 0, 0,  -x1*x2, -x1*y2, -x1*z2);
		A.push_back(Ai);
	}
	std::cout << "A" << std::endl << A << std::endl << std::endl;
	cv::Mat_<double> w, u, vt;  // vt(9, 9)
	cv::SVD::compute(A, w, u, vt, cv::SVD::FULL_UV);
	////////////////////   h1, h2, h3 =  [vt00, vt10, vt20, ... , vt80]
	////////////////////   H = [h1, h2, h3]
	std::cout << "w=" 	<< w 	<< endl << endl;
	std::cout << "u=" 	<< u 	<< endl << endl;
	std::cout << "vt=" 	<< vt 	<< endl << endl;
	cv::Mat H = (cv::Mat_<double> (3, 3)  <<  vt(8,0), vt(8,1), vt(8,2), vt(8,3), vt(8,4), vt(8,5), vt(8,6), vt(8,7), vt(8,8) );
	// cv::Mat H = (cv::Mat_<double>(2,3) << vt(8,0) , vt(8,1) , vt(8,2) ,
	//  vt(8,3) , vt(8,4), vt(8,5) );
	 std::cout << "affin matrix" <<  std::endl << H << std::endl;




	 //////////////////////// image transform
	 cv::Mat img1 = cv::imread("img1.JPG", 0);
	 cv::Mat img2 = cv::imread("img2.JPG", 0);

 	 cv::Mat out_img = cv::Mat::zeros(img1.rows, img1.cols, img1.type());
	 AffinTransformer affin;
	 cv::Matx33d mat = affin.Translation(0,0);
	 // warpAffine(img, out_img, H, out_img.size());
	 out_img = affin.TransformE(img1, H, 0);
	 // out_img = affin.TransformL(img, H);
	 cv::imwrite("out.jpg", out_img);

	return 0;
}


