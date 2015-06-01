#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../asign0508/ppoint.hpp"
#include <opencv2/opencv.hpp>
#include "../asign0522/8_geometry4/skip_comments.hpp"

#include "camera.hpp"

int main() {
	////////// make skel
	SKEL skel = SKEL("../asign0522/cg.skel");
	SKEL skel2 = SKEL("world.skel");
	SKEL skel3 = SKEL("T.skel");
	SKEL skel4 = SKEL("T_.skel");
	vector<SKEL> skels;
//	skels.push_back(skel);
	skels.push_back(skel2);
	skels.push_back(skel3);
	skels.push_back(skel4);
	/////////// make camera
	Camera c1;
	Camera c2;
	c1.set_size(500, 500);
	c2.set_size(500, 500);
	c1.set_intrinsic(0.5, 3., 2., 0., 1., 1.); // f, ku, kv, s, u0, v0
	c2.set_intrinsic(2. , 1., 1., 0., 1., 1.);

	// make location
	Matx44d R = Translation(-1, -1, -10);
	Matx44d R2 = Translation(1, -10, -1);
	R2 = R2*RotationY(3.14/2);
	c1.set_location(R);
	c2.set_location(R2);

	Matx44d T  = Translation(0, 0, 0.1); 
	Matx44d T2  = RotationX(0.01);
	//R = R*T;

	for (unsigned i=0; i<100; i++) {
		cv::Mat img(500, 500, CV_8UC1, 1);
		c1.transform(T);
		c2.transform(T2);
		//cout << i << endl;
		draw_skels_perspective(img, skels, c1, 255);
		Point2f P(1, 0);
		Scalar S(255);
		putText(img, "X", P, FONT_HERSHEY_SIMPLEX, 5, S);
		stringstream file_name;
		file_name <<"img/camera" << std::setw( 3 ) << std::setfill( '0' ) << i   << ".png";
	  cv::imwrite(file_name.str(), img);
	}
}

