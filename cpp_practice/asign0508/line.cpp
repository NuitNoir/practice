/*
 * line,.cpp
 *
 *  Created on: 2015/05/08
 *      Author: kyota
 */
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
#include "ppoint.hpp"
#include "../asign0423/drawLine.hpp"
//#include "transform.hpp"
#define EPS 0.000001

using namespace tutor;
using namespace cv;
using namespace std;

//void draw_circle(cv::Mat& img, PPoint2i &p, unsigned char r, unsigned char l);
void draw_line(cv::Mat &img, const PPoint2d &p, unsigned char l);
void mark(cv::Mat& img, PPoint2i &p, unsigned char l);
void draw_markedline(cv::Mat &img, PPoint2i &p1, PPoint2i &p2, unsigned char l);
PPoint2d cross_product(PPoint2i &p1, PPoint2i &p2);
int draw_2lines_mark(cv::Mat &img, PPoint2d &p1, PPoint2d &p2, unsigned char l);
PPoint2i cross_product(PPoint2d &p1, PPoint2d &p2);

int main() {
  //cv::Mat img = cv::imread("../cat/cat1.ppm", 0);
  cv::Mat img(500, 500, CV_8UC1, 1);
  unsigned char light = 255;
  // draw line
  PPoint2d p(-5, 2, -3);
  //draw_line(img, p, light);
  // draw mark
  PPoint2i p1(150, 500, 2);
  // mark(img, p1, 255);

  PPoint2i p2(320, 300, 2);
  // draw_markedline(img, p1, p2, light);

  PPoint2d p10(-10, 20, 10);
  PPoint2d p11(25, -150, 3000);
  draw_2lines_mark(img, p10, p11, light);
  cv::imwrite("lines.png", img);
}

int draw_2lines_mark(cv::Mat &img, PPoint2d &p1, PPoint2d &p2, unsigned char l) {
  draw_line(img, p1, l);
  draw_line(img, p2, l);
  PPoint2i p3(0,0);
  p3 = cross_product(p1, p2);
  mark(img, p3, l);

  return 0;
}
void draw_markedline(cv::Mat &img, PPoint2i &p1, PPoint2i &p2, unsigned char l) {
  mark(img, p1, l);
  mark(img, p2, l);
  PPoint2d p3 = cross_product(p1, p2);
  draw_line(img, p3, l);
  //draw_line(img, p1[0], p1[1], p2[0], p2[1], l);
}
void draw_line(cv::Mat &img, const PPoint2d &p, unsigned char l) {

	// P = (a, b, c)
	//	a*x + b*y + c = 0
	//		y = -a/b*x - c/b
	//		y = -p[0]/p[1]*x - p[2]/p[1]
	//
	//		x = -b/a*y - c/a
	//		x = -p[1]/p[0]*y - p[2]/p[0]
  double slope;
  double intercept;
  int height = img.rows;
  int width = img.cols;
  int MAX_VAL = 1000000000;
  slope = p[0]/p[1];

  // SLOPE SIZE < abs(1)
  if (abs(slope) <= 1) {
    int x = 0;
    slope = -p[0]/p[1];
    intercept = p[2]/p[1];
    cout << slope << ' ' << intercept << endl;
    double y = slope*x - intercept;
    //slope < 1 to right
    for (int x=0; x<width; x++) {
      //			std::cout << slope << std::endl;
      //			std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;
      //y += slope;
      y = slope*x - intercept;
    	if (y < 0 || y > height) continue;
      img.at<unsigned char>(static_cast<int>(y + 0.5), x) = l;
    }
  } else {
    // when slope > abs(1)
    slope = -p[1]/p[0];
    intercept = p[2]/p[0];
    cout << slope << ' ' << intercept << endl;
    int y = 0;
    double x = slope*y - intercept;
    // slope > 1 to right
    for (int y=0; y<height; y++) {
      //			std::cout << slope << std::endl;
      //			std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;
      //x += slope;
      x = slope*y - intercept;
    	if (x > width || x < 0) continue;
      img.at<unsigned char>(y, static_cast<int>(x + 0.5)) = l;
    }
  }
}
PPoint2i cross_product(PPoint2d &p1, PPoint2d &p2) {
  //	cv::Mat<double> m1(p1[0], p1[1], p1[2]);
  //	cv::Mat<double> m2(p2[0], p2[1], p2[2]);
  //	cv::Mat m3(0, 0, 0);
  //	m3 = m1.cross(m2);
  //PPoint2i p3(m3.at<int>(0),m3.at<int>(1), m3.at<int>(2));
  PPoint2i p3(0, 0, 1);
  cout << p1 << endl;
  cout << p2 << endl;
  p3[0] = (int)(p1[1]*p2[2] - p1[2]*p2[1]);
  p3[1] = (int)(p1[2]*p2[0] - p1[0]*p2[2]);
  p3[2] = (int)(p1[0]*p2[1] - p1[1]*p2[0]);

  cout << p3 << endl;
  return p3;
}

PPoint2d cross_product(PPoint2i &p1, PPoint2i &p2) {
  PPoint2d p3(0, 0);
  p3[0] = (double)(p1[1]*p2[2] - p1[2]*p2[1]);
  p3[1] = (double)(p1[2]*p2[0] - p1[0]*p2[2]);
  p3[2] = (double)(p1[0]*p2[1] - p1[1]*p2[0]);

  cout << p3 << endl;
  return p3;
}

void mark(cv::Mat& img, PPoint2i &p, unsigned char l) {
  int r = 10;
  //draw_circle(img, p, r, l);
  //	draw_circle(img, p[0], p[1], 10, l);
  //	draw_line(img, p[0]-r/2, p[1]-r/2, p[0]+r/2, p[1]+r/2, l);
  //	draw_line(img, p[0]-r/2, p[1]+r/2, p[0]+r/2, p[1]-r/2, l);
  double x = p.x();
  double y = p.y();
  cout << p << endl;
  if (x >= 0 && y >= 0 && x <= img.rows && y <= img.cols) {
	draw_circle(img, x, y, 10, l);
	draw_line(img, x - r / 2, y - r / 2, x + r / 2, y + r / 2, l);
	draw_line(img, x - r / 2, y + r / 2, x + r / 2, y - r / 2, l);
  }
}
