/*
 * ls_practice.cpp
 *
 *  Created on: 2015/04/17
 *      Author: kyota
 */

// uBLAS Basic
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
// SVD
//#include <boost/numeric/bindings/lapack/gesdd.hpp>
//#include <clapack.hpp>
//#include <umlapack.hpp>
#include <boost/bind.hpp>
#include <boost/numeric/ublas/fwd.hpp>
// LU factorize
#include <boost/numeric/ublas/lu.hpp>
// TR1
#include <boost/tr1/array.hpp>
#include <boost/tr1/tuple.hpp>
#include <boost/tr1/random.hpp>
#include "/home/kyota/boost-sandbox/numeric_bindings/boost/numeric/bindings/lapack.hpp"
#include <numeric/bindings/tools/templates/lapack.hpp>


int main() {
	//initial vector
	std::tr1::array<double, 9> x = {{2.8, 2.9, 3.0, 3.1, 3.2, 3.2, 3.2, 3.3, 3.4}};
	std::tr1::array<double, 9> y = {{ 30,  26,  33,  31,  33,  35,  37,  36,  33}};
	std::tr1::array<double, 9> x2;
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
	boost::numeric::ublas::matrix<double> mat_a(2, 2);
	mat_a(0, 0) = x2_sum;
	mat_a(0, 1) = x_sum;
	mat_a(1, 0) = x_sum;
	mat_a(1, 1) = x.size();

	// vector b
	boost::numeric::ublas::vector<double> b = {{xy_prod, y_sum}};

	//vector u
	boost::numeric::ublas::vector<double> u = {{0, 0}};

	// invert matrix
	//math::svd(mat_a);
	boost::numeric::ublas::matrix<double> mat_a_inv(2,2);
	double det = mat_a(0,0)*mat_a(1,1) - mat_a(0,1)*mat_a(1,0);
	mat_a_inv(0,0) = mat_a(1,1);
	mat_a_inv(1,1) = mat_a(0,0);
	mat_a_inv(0,1) = -mat_a(0,1);
	mat_a_inv(1,0) = -mat_a(1,0);
	mat_a_inv = mat_a_inv/det;

	boost::numeric::bindings::lapack::svd();

	boost::numeric::ublas::prod(b, mat_a_inv);


}




