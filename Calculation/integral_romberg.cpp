#include <iostream>
#include <cmath>

using namespace std;
double integral_romberg(double a, double b, int m);

int main(void) {

	return 1;
}

double integral_romberg(double a, double b, int m) {
	double t[31][31], x[1001], h, s;
	int i, j, n, l, k, bun;

	cout << "積分区間を入力してください。[a, b]で積分します。" << endl;
	cin >> a;
	cin >> b;
	cout << "段階数 M ( M < 11 : 分点数は２のM乗）を入力して" << endl;
	cin >> m;

	for (j = 0; j <= m + 1; j++) {
		for (i = 0; i <= m + 1; i++) {
			t[i][j] = 0;
		}
	}
	//台形公式
	for (k = 1; k <= m + 1; k++) {
		n = pow(2, k);
		h = (b - a) / n;
		for (i = 1; i <= n - 1; i++) {
			s += sqrt(1.0 - x[i + 1] * x[i + 1]);
		}
		t[1][k] = (sqrt(1. - x[1] * x[1]) + 2. * s
				+ sqrt(1. - x[n + 1] * x[n + 1])) * h / 2;
	}
	//積分値の外挿
	for (l = 1; l <= m; l++) {
		n = pow(4, (l + 1));
		for (k = 0; k <= m; k++) {
			t[l + 1][k + 1] = (n * t[l][k + 2] - t[l][k + 1]) / (n - 1);
		}
	}

	bun = pow(2, k);
	cout << "**計算結果**" << endl;
	cout << "下端 = " << a << "　上端 = " << b << "　分点数 = " << bun << endl;
	cout << "積分値 = " << t[m + 1][1] << endl;

	return t[m + 1][1];
}

double integral_romb() {
	double a, b, t[31][31], x[1001], h, s;
	int m, i, j, n, l, k, bun;

	cout << "積分区間を入力してください。[a, b]で積分します。" << endl;
	cin >> a;
	cin >> b;
	cout << "段階数 M ( M < 11 : 分点数は２のM乗）を入力して" << endl;
	cin >> m;

	for (j = 0; j <= m + 1; j++) {
		for (i = 0; i <= m + 1; i++) {
			t[i][j] = 0;
		}
	}
	//台形公式
	for (k = 1; k <= m + 1; k++) {
		n = pow(2, k);
		h = (b - a) / n;
		for (i = 1; i <= n - 1; i++) {
			s += sqrt(1.0 - x[i + 1] * x[i + 1]);
		}
		t[1][k] = (sqrt(1. - x[1] * x[1]) + 2. * s
				+ sqrt(1. - x[n + 1] * x[n + 1])) * h / 2;
	}
	//積分値の外挿
	for (l = 1; l <= m; l++) {
		n = pow(4, (l + 1));
		for (k = 0; k <= m; k++) {
			t[l + 1][k + 1] = (n * t[l][k + 2] - t[l][k + 1]) / (n - 1);
		}
	}
	bun = pow(2, k);
	cout << "**計算結果**" << endl;
	cout << "下端 = " << a << "　上端 = " << b << "　分点数 = " << bun << endl;
	cout << "積分値 = " << t[m + 1][1] << endl;

	return t[m + 1][1];
}
