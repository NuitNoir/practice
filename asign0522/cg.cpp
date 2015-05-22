
#include <iostream>
#include <istream>
#include <sstream>
#include    <iomanip>
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
int hide_line(PPoint2d p1,PPoint2d p2,PPoint2d p3);

int main() {
	//cv::Mat img(500, 500, CV_8UC1, 1);

	SKEL skel = SKEL("cg.skel");
	//Matx44d R = Translation(1, 0, 1);
	//Matx44d R = RotationX(1);
	//Matx44d R = RotationY(1);
	//Matx44d R = RotationZ(0.5);

	//cout << R(1,1) << endl;
	//skel.transform(R);
	//cout << R(1,1) << endl;
	//skel.transform(Translation(0, 0, 0));
	//skel.transform(M);
	//cout << Translation(1, 0, 0) << endl;
	//cout << M(0,3) << endl;

	for (int i=0; i<30; i++) {
		cv::Mat img(500, 500, CV_8UC1, 1);
		Matx44d R = RotationX(1);
		skel.transform(R);
		draw_skel_perspective(img, skel);
		stringstream file_name;
		file_name << "img/cg_" << std::setw( 3 ) << std::setfill( '0' ) <<i  << ".png";
		cv::imwrite(file_name.str(), img);
	}
}

PPoint2d perspective_projection(const PPoint3d &p3d) {
	double f = 30;
	cout << p3d[0] << endl;
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

	for (unsigned i=0; i < vertices.size(); i++) {
		cout << vertices[i][0] << endl;
		projected_v.push_back( perspective_projection(vertices[i]) );
	}

	int hide_flag = 0;
	//skel.print_polylines();
	vector<Polyline> lines = skel.polylines();
	for (unsigned i=0; i<lines.size(); i++) {
		for (unsigned j=1; j<lines[i][0]; j++) { //lines[i][0] has passing point num.
			//draw_line(img, projected_v[lines[i][j]][0], projected_v[lines[i][j]][1],
			//		projected_v[lines[i][j+1]][0], projected_v[lines[i][j+1]][1], 255);
			double f = 4, bias = 250;

			//cout << "vertices num " <<  vertices.size() << endl;
			//cout << "projected_v num " << projected_v.size() << endl;
			//cout << lines[i][1] << lines[i][2] << lines[i][3] << endl;
			if (lines[i][0] >= 3) {
				hide_flag = hide_line(projected_v[lines[i][1]], projected_v[lines[i][2]], projected_v[lines[i][3]] );
				cout << hide_flag << endl;
			}
			//if ( (projected_v[lines[i][j]][2] > EPS || projected_v[lines[i][j+1]][2] > EPS) && hide_flag != 1) {
			if (hide_flag != 1) {
				double p1x, p1y;
				p1x = projected_v[lines[i][j]].x()*f + bias;
				p1y = projected_v[lines[i][j]].y()*f + bias;
				//cout << p1x << ' ' << p1y << endl;
				Point2f p1(p1x, p1y);
				Point2f p2(projected_v[lines[i][j+1]].x()*f + bias, projected_v[lines[i][j+1]].y()*f + bias);
				//line(img, projected_v[lines[i][j]], projected_v[lines[i][j+1]], 255);
				cout << p1 << ' ' << p2 << endl;
				line(img, p1, p2, 255);
			}
			hide_flag = 0;
		}
	}
}

/*
 * ***  return value *************
 * when hide line return 1 (true)
 * when doesn't hide line return 0 (false)
 * ****************************************
 */
int hide_line(PPoint2d p0, PPoint2d p1, PPoint2d p2) {
	Point2f vec1(p1[0]-p0[0], p1[1]-p0[1]);
	Point2f vec2(p2[0]-p0[0], p2[1]-p0[1]);

	double inner_product = vec1.ddot(vec2);

	cout << inner_product << endl;

	if (inner_product > 0) {
		return 1;
	} else {
		return 0;
	}
}

// judge size
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

