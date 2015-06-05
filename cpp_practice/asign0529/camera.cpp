#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../asign0508/ppoint.hpp"
#include <opencv2/opencv.hpp>
#include <cmath>
#include "../asign0522/8_geometry4/skip_comments.hpp"

#include "camera.hpp"

int main() {
	//////////    make skel
	SKEL skel = SKEL("../asign0522/cg.skel");
	SKEL skel2 = SKEL("world.skel");
	SKEL skel3 = SKEL("T.skel");
	SKEL skel5 = SKEL("L1.skel");
	SKEL skel6 = SKEL("L2.skel");
	SKEL skel7 = SKEL("L3.skel");
	SKEL skel8 = SKEL("T2.skel");
	SKEL skel9 = SKEL("cube.skel");
;	vector<SKEL> skels;
	// skels.push_back(skel);
	// skels.push_back(skel2);
	// skels.push_back(skel3);
	// skels.push_back(skel5);
	// skels.push_back(skel6);
	// skels.push_back(skel7);
	// skels.push_back(skel8);
	skels.push_back(skel9);

	/////////// make camera
	Camera c1;
	Camera c2;
	c1.set_size(500, 500);
	c2.set_size(500, 500);
	c1.set_intrinsic(1, 1., 1., 0., 1., 1.); // f, ku, kv, s, u0, v0
	// c2.set_intrinsic(2. , 1., 1., 0., 1., 1.);
	double z0 = -10;
	double dz = 0.1;

	// make location
	Matx44d R = Translation(-3, -3, z0);
	// Matx44d R2 = Translation(1, -10, -1);
	Matx44d R3 = RotationY(0.1);
	// R = R3*RotationY(3.14/2);
	R = R3*R;
	c1.set_location(R);
	// c2.set_location(R2);

	Matx44d T  = Translation(0, 0, dz);
	 // Matx44d T = RotationX(0.1);

	for (unsigned i=0; i<100; i++) {
			 // Matx44d T  = Translation(cos(i), sin(i), dz);
		cv::Mat img(500, 500, CV_8UC1, 1);
		c1.transform(T);
		// c2.transform(T2);
		//cout << i << endl;
		c1.draw_skels_perspective(img, skels, (unsigned char)255);
		// Point2f P(1, 0);
		// Scalar S(255);
		// putText(img, "X", P, FONT_HERSHEY_SIMPLEX, 5, S);
		stringstream file_name;
		file_name <<"img/camera" << std::setw( 3 ) << std::setfill( '0' ) << i   << ".png";
	  cv::imwrite(file_name.str(), img);
	}
}
