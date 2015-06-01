/*
 * integral_simpson.c


 *
 *  Created on: 2015/05/16
 *      Author: kyota
 */

#include <iostream>
#include <cmath>

using namespace std;
double integral_simpson(double, double, int);

int main(void) {
	double integral;

	//integral = integral_simp();
	double a, b; // 区間[a, b]
	int n;		// 区分点 n
	int ns[8] = {1, 5, 10, 30, 50, 100, 500, 1000};
	n = 5;
	a = 0;
	b = sqrt(0.5);
	double pi;
	int length = sizeof(ns) / sizeof(ns[0]);
	for (int i = 0; i < length ; i++) {
		integral = integral_simpson(a, b, ns[i]);
		pi = (integral - 1/4.0 ) * 8;
		cout << "pi = " << pi << "  区分点n = " << ns[i] << endl;
	}

	return 1;
}

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

double integral_simp() {
	double a, b, y2, y4, h, ya, x, yb, s;
	int n, i;

	cout << "積分を入力してください。[a, b]で積分します。" << endl;
	cin >> a;
	cin >> b;
	cout << "分点数を入力してください" << endl;
	cin >> n;

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
