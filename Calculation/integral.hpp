/*
 * integral.hpp
 *
 *  Created on: 2015/05/16
 *      Author: kyota
 */

#ifndef INTEGRAL_HPP_
#define INTEGRAL_HPP_

double integral_simpson(double, double, int);

double integral_simpson(double a, double b, int n) {
	double y2, y4, h, ya, x, yb, s;
	int i;

	y2 = 0.;
	y4 = 0.;
	h = .5*(b - a)/n;
	x = a;
	ya = sqrt(1. - x*x);
	//cout << ya << a << endl;
	x = b;
	yb = sqrt(1. - x*x);
	//cout << yb << b << endl;

	x = a + h;
	for (i=1; i<=n; i++) {
		y4 = y4 + sqrt(1. - x*x);
		x = x + 2.*h;
	}
	x = a + 2.*h;
	for (i=1; i <= n-1; i++) {
		y2 = y2 + sqrt(1. - x*x);
		x = x + 2.*h;
	}
	//cout << ya << yb << y2 << y4 << endl;
	s = h * (ya + yb + 2.*y2 + 4.*y4) / 3.;
	cout << "下端 = " << a << "　上端 = " << b << "　分点数 = " << n << endl;
	cout << "積分値" << s << endl;

	return s;

}

#endif /* INTEGRAL_HPP_ */
