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
#include "cg.hpp"
#define EPS 0.01
using namespace tutor;
using namespace cv;
using namespace std;


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

  for (int i=0; i<100; i++) {
    cv::Mat img(500, 500, CV_8UC1, 1);
    Matx44d R = RotationZ(2*3.14/100);
    skel.transform(R);
    R = RotationX(2*3.14/100);
    skel.transform(R);
    draw_skel_perspective(img, skel);
    stringstream file_name;
    file_name << "img/cg_" << std::setw( 3 ) << std::setfill( '0' ) <<i  << ".png";
    cv::imwrite(file_name.str(), img);
  }
}
