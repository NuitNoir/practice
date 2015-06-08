#include <array>
#include <cmath>
#include <iostream>
#define ARRMAX 125

void thomas(int il, int iu, double *a, double *b, double *c, double *d);


int  main(void) {
	double a[ARRMAX], b[ARRMAX], c[ARRMAX], d[ARRMAX];
	// std::array<double, ARRMAX> a, b, c, d;

	double h, u, err, x;
	double k=2, m=1;
	int n, i;
	h = 1./n;


	std::cout << "initialize" << std::endl;
	for (int i=2; i<=n; i++) {
		a[i] = 1.0 - h;
		b[i] = 2.*pow(h,2) - 2.;
		c[i] = 1.0 + h;
		d[i] = 4.0*(i-1)*pow(h,3);
	}
	std::cout << "thomas" << std::endl;
	thomas(k,n,a,b,c,d);

	for (int i=2; i<=n; i++) {
		x = h*(i-1);
		u = 2*exp(-x)*(cos(m)*sin(x)/sin(m)) + 2.0*x-2;
		err = (d[i] - u)/u*100.0;
	}
	std::cout << x << ' ' << d[i] << ' ' << u << ' ' << err << std::endl;
	return 1;
}

void thomas(int il, int iu, double *a, double *b, double *c, double *d) {
	double r;
	int ip, i, j;
	ip = il + 1;

	for (int i=ip; i<=iu; i++) {
		r		= a[i]/b[i-1];
		b[i]	= b[i] - r*c[i-1];
		d[i]	= d[i] - r*d[i-1];
	}

	d[iu] = d[iu]/b[iu];
	for (int i=ip; i<=iu; i++) {
		j 		= iu - i + il;
		d[j]	= (d[j] - c[j]*d[j+1] )/b[j];

	}
}
