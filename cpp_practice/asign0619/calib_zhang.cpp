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
#define N  3

cv::Mat_<double> homo_graphy(std::vector<cv::Point3d>  x_1,  std::vector<cv::Point3d>  x_2 ) ;
std::vector<cv::Point3d> read_positions(char*  file_name) ;

int main() {
	std::vector<cv::Point3d>x_1;
	std::vector<cv::Point3d>x_2 ;

	char* file_names[] = {"chess1.txt", "chess2.txt", "chess3.txt"};
	x_1 = read_positions("world.txt");

	cv::Mat V;
	cv::Mat w, u, vt;  // vt(9, 9)	
	////////  read imgs
	for (unsigned j=0; j < 3; j++) {
		// std::cout << file_names[i] << std::endl;
		x_2 = read_positions(file_names[j]);
		std::cout << "chess" << j+1 << "\t=" << x_2 << std::endl;
		/////////  homography matrix
		cv::Mat_<double> H = homo_graphy(x_1, x_2);	

		 ////////////////////  Vi = (v12, (v11 - v22) )
		 cv::Mat v_12 = (cv::Mat_<double>(1,6) <<  H(0,0)*H(0,1),  			    H(0,0)*H(1,1) + H(1,0)*H(0,1),    H(1,0)*H(1,1),
										H(2,0)*H(0,1) + H(0,0)*H(2,1),    H(2,0)*H(1,1) + H(1,0)*H(2,1),    H(2,0)*H(2,1) ) ;
		 cv::Mat v_11 = (cv::Mat_<double>(1,6) << H(0, 0)*H(0,0), 	H(0,0)*H(1,0)*1,  H(1,0)*H(1,0),
										H(2,0)*H(0,0)*1,  H(2,0)*H(1,0)*1,  H(2,0)*H(2,0));
		 cv::Mat v_22 = (cv::Mat_<double>(1,6) << H(0, 1)*H(0,1) ,      H(0,1)*H(1,1) *1,  H(1,1)*H(1,1),
										H(2,1)*H(0,1)*1 ,   H(2,1)*H(1,1)*1,   H(2,1)*H(2,1));

		 std::cout << "v_12\t=" << v_12 << std::endl;
		 std::cout << "v_11\t=" << v_11 << std::endl;
		 std::cout << "v_22\t=" << v_22 << std::endl;
		 /////////////////   V = sum(Vi)
		 V.push_back(v_12);
		 cv::Mat v_11_22 = v_11 - v_22;
		 V.push_back(v_11_22);
		 std::cout << "" << std::endl;
	}
	 ////////////////// estimation of intrinsic camera parameter 
	 //////////////////// V^t*V least gen
	std::cout << "V\t=" <<  V << std::endl;
	cv::SVD::compute(V, w, u, vt, cv::SVD::FULL_UV);
	std::cout << "w\t=" <<  w << endl;
	std::cout << "u\t=" << u << endl;
	std::cout << "vt\t=" << vt << endl;

	cv::Mat_<double>  b =  vt.row(5);
	std::cout << "b\t= " << b << std::endl;

	////////  B = B11, B12, B13, 
	/////////        B12, B22, B23, 
	 ////////          B13, B23, B33
	//////// b = B11, B12, B22, B13, B23, B33
	////////         B[0][0], B[1][0], B[2][0], B[2][1], B[2][2]
	cv::Mat_<double> B = (cv::Mat_<double>(3,3)  <<  b(0,0), b(0,1), b(0,3), 
									b(0,1), b(0,2), b(0,4), 
									b(0,3), b(0,4), b(0,5) );
	std::cout << "B\t= " << B << std::endl;

	/////////  intrinsic parameter
	double u0, v0, alpha, beta, gamma, lamda;
	v0 		= (B(0, 1)*B(0,2) - B(0,0)*B(1,2)) / (B(0,0)*B(1,1) - B(0,1)*B(0,1)) ;
	lamda 	= B(2,2) - (B(0,2)*B(0,2) + v0*(B(0,1)*B(0,2) - B(0,0)*B(1,2))) / B(0,0) ;
	beta	= sqrt( lamda*B(0,0) / (B(0,0)*B(1,1) - B(0,1)*B(0,1)) ) ;
	alpha	= sqrt(lamda/B(0,0)) ;
	gamma	=  -B(0,1)*beta / B(0,0) ; //-(B(0,1)*alpha*alpha*beta/lamda) ;
	u0		= (gamma*v0 / alpha) -  B(0,2) / B(0,0) ;  //(B(0,2)*alpha*alpha / lamda) ;
	std::cout << "lamda\t=" << lamda << std::endl << "alpha\t=" << alpha << std::endl;
	cv::Mat K =  (cv::Mat_<double>(3,3)  <<  alpha, gamma, u0,		0, beta, v0,	0, 0, 1);
	std::cout << "K\t= " << K << std::endl;

	return 0;
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

cv::Mat_<double> homo_graphy(std::vector<cv::Point3d>  x_1,  std::vector<cv::Point3d>  x_2 ) {
	cv::Mat A;
	cv::Mat w, u, vt;  // vt(9, 9)	
	for (int i=0; i<(int)x_1.size(); i++) {
		// std::cout << "i=" << i << ",  x_1=" << x_1[i] << ",  x_2=" << x_2 [i]<< std::endl;
		int x1 = x_1[i].x, y1 = x_1[i].y, w1 = x_1[i].z; 
		int x2 = x_2[i].x, y2 = x_2[i].y, z2 = x_2[i].z; 
		cv::Mat  Ai = (cv::Mat_<double> (2,9)  << 0, 0,0,  				-w1*x2, -w1*y2, -w1*z2,   	y1*x2, y1*y2, y1*z2,
										w1*x2, w1*y2, w1*z2, 	0, 0, 0,  				-x1*x2, -x1*y2, -x1*z2);
		A.push_back(Ai); 
	}
	// std::cout << "A" << std::endl << A << std::endl;
	cv::SVD::compute(A, w, u, vt, cv::SVD::FULL_UV);
	////////////////////   h1, h2, h3 =  [vt00, vt10, vt20, ... , vt80]
	////////////////////   H = [h1, h2, h3]
	// std::cout << "w=" <<  w << endl;
	// std::cout << "u=" << u << endl;
	// std::cout << "vt=" << vt << endl;
	cv::Mat H = (cv::Mat_<double> (3, 3)  <<  vt.at<double>(8,0) , vt.at<double>(8,1) , vt.at<double>(8,2) , vt.at<double>(8,3) , vt.at<double>(8,4) , 
		vt.at<double>(8,5) , vt.at<double>(8,6) , vt.at<double>(8,7) , vt.at<double>(8,8)  );
	 std::cout << "H\t=  " << H << std::endl;
	 return H;
}