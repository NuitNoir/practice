
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
#include "skel.hpp"
#include "../asign0508/ppoint.hpp"
#include "../asign0423/drawLine.hpp"
#include "transform_3d.hpp"
#define EPS 0.01
using namespace tutor;
using namespace cv;
using namespace std;

PPoint2d perspective_projection(const PPoint3d &p);
void draw_skel_perspective(Mat &img, const SKEL &s);
vector<int> splits(string str, char delimiter);

int main() {
	cv::Mat img(500, 500, CV_8UC1, 1);

	SKEL skel = SKEL("cg.skel");
	//Matx44d R = Translation(1, 0, 1);
	Matx44d R = RotationX(2);
	//Matx44d R = RotationY(6);
	//Matx44d R = RotationZ(0.1);

	//cout << R(1,1) << endl;
	skel.transform(R);
	//cout << R(1,1) << endl;
	//skel.transform(Translation(0, 0, 0));
	//skel.transform(M);
	//cout << Translation(1, 0, 0) << endl;
	//cout << M(0,3) << endl;
	draw_skel_perspective(img, skel);

	cv::imwrite("cg.png", img);
}

PPoint2d perspective_projection(const PPoint3d &p3d) {
	double f = 50;
	Matx34d M(f, 0, 0, 0,
			0,f, 0, 0,
			0, 0, 1, 0);
	PPoint2d p2d = M*p3d;
	return p2d;
}

void draw_skel_perspective(Mat &img, const SKEL &skel) {
	//skel.print_vertices();
	vector<PPoint3d> vertices = skel.vertices();
	vector<PPoint2d> projected_v;
/*	int x[2] = {1000, -1000}; // min, max
	int y[2] = {1000, -1000}; // min, max
	int z[2] = {1000, -1000}; // min, max
	for (unsigned i; i<vertices.size(); i++) {
		// x min, max
		if (x[0] > vertices[i][0]) {
			x[0] = vertices[i][0];
		} else if (x[1] > vertices[i][0]) {
			x[1] = vertices[i][0];
		}
		// y
		if (y[0] > vertices[i][1]) {
			y[0] = vertices[i][1];
		} else if (y[1] > vertices[i][1]) {
			y[1] = vertices[i][1];
		}
		// z
		if (z[0] > vertices[i][2]) {
			z[0] = vertices[i][2];
		} else if (z[1] > vertices[i][2]) {
			z[1] = vertices[i][2];
		}
	}*/
	for (unsigned i; i<vertices.size(); i++) {
/*		double f = 150, bias = 1.5;
		vertices[i][0] = (vertices[i][0] + bias)*f;
		vertices[i][1] = (vertices[i][1] + bias)*f;
		vertices[i][2] = (vertices[i][2] + bias)*f;*/
		//vertices[i][2] = vertices[i][2] + 2;
		projected_v.push_back( perspective_projection(vertices[i]) );
	}

	//skel.print_polylines();
	vector<Polyline> lines = skel.polylines();
	for (unsigned i=0; i<lines.size(); i++) {
		for (unsigned j=1; j<lines[i][0]; j++) { //lines[i][0] has passing point num.
			//draw_line(img, projected_v[lines[i][j]][0], projected_v[lines[i][j]][1],
			//		projected_v[lines[i][j+1]][0], projected_v[lines[i][j+1]][1], 255);
			double f = 3, bias = 100;
			if (projected_v[lines[i][j]][2] > EPS || projected_v[lines[i][j+1]][2] > EPS) {
				double p1x, p1y, p2x, p2y;
				p1x = projected_v[lines[i][j]].x()*f + bias;
				p1y = projected_v[lines[i][j]].y()*f + bias;
				//cout << p1x << ' ' << p1y << endl;
				Point2f p1(p1x, p1y);
				Point2f p2(projected_v[lines[i][j+1]].x()*f + bias, projected_v[lines[i][j+1]].y()*f + bias);
				//line(img, projected_v[lines[i][j]], projected_v[lines[i][j+1]], 255);
				cout << p1 << ' ' << p2 << endl;
				line(img, p1, p2, 255);
			}
		}
	}
}

int hide_line() {

	double innner_product = vec1.dotprod(vec2);
}


