/*
 * ls_cv.cpp
 *
 *  Created on: 2015/04/17
 *      Author: kyota
 */
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

int main() {
	//initial vector
	std::vector<double> x = {{2.8, 2.9, 3.0, 3.1, 3.2, 3.2, 3.2, 3.3, 3.4}};
	std::vector<double> y = {{ 30,  26,  33,  31,  33,  35,  37,  36,  33}};
	std::vector<double> x2;
	double x_sum = 0;
	double y_sum = 0;
	double xy_prod = 0;
	double x2_sum = 0;

	// prepare values
	for (int i=0; i<x.size(); i++) {
			x_sum += x[i];
			x2[i] = x[i]*x[i];
			x2_sum += x2[i];
			y_sum += y[i];
			xy_prod += x[i]*y[i];
	}

	// Matrix A
	//boost::numeric::ublas::matrix<double> mat_a(2, 2);
	cv::Mat<double> mat_a(2, 2);
	mat_a(0, 0) = x2_sum;
	mat_a(0, 1) = x_sum;
	mat_a(1, 0) = x_sum;
	mat_a(1, 1) = x.size();

	// vector b
	std::vector<double> b = {{xy_prod, y_sum}};

	//vector u
	std::vector<double> u = {{0, 0}};

	// invert matrix
	//math::svd(mat_a);
	cv::Mat<double> mat_a_inv(2,2);
	double det = mat_a(0,0)*mat_a(1,1) - mat_a(0,1)*mat_a(1,0);
	mat_a_inv(0,0) = mat_a(1,1);
	mat_a_inv(1,1) = mat_a(0,0);
	mat_a_inv(0,1) = -mat_a(0,1);
	mat_a_inv(1,0) = -mat_a(1,0);
	mat_a_inv = mat_a_inv/det;

	cv::Mat<double> mats = cv::SVD(mat_a);

	std::cout << "mat1 = " << mats;

}

