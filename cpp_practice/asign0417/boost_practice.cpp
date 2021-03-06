/*
 * boost_practice.cpp


 *
 *  Created on: 2015/04/17
 *      Author: kyota
 */

// uBLAS
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
// LU factorize
#include <boost/numeric/ublas/lu.hpp>
// TR1
#include <boost/tr1/array.hpp>
#include <boost/tr1/tuple.hpp>
#include <boost/tr1/random.hpp>

using namespace boost::numeric::ublas;
//namespace ub = boost::numeric::ublas;

int main() {
	matrix<double> mat_d(3,4);

	boost::numeric::ublas::vector<double> vec_d(4);

	std::cout << "mat1 = " << mat_d << std::endl;

	std::tr1::array<int, 5> arr = {{1,2,3,4,5}};

	std::tr1::tuple<std::string, double, double> kyota("matsu", 163.2, 57.4);

}

