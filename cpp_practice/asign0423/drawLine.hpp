#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

//using namespace std;
void draw_line_p(cv::Mat &img, const PPoint2d &p, unsigned char l);
void draw_line(cv::Mat&, int, int, int, int, unsigned char);
void draw_line_bres(cv::Mat&, int, int, int, int, unsigned char);
void draw_circle(cv::Mat&, int, int, unsigned, unsigned char);

class LineDrawer {
	void draw_line_p(cv::Mat &img, const PPoint2d &p, unsigned char l) {

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
	  // cv::imwrite("img.jpg", img);
	}

	void draw_line_bres(cv::Mat &dst, int x1, int y1, int x2, int y2, unsigned char l) {
		double slope = 0;
		double err = 0;
		int height = dst.rows;
		int width = dst.cols;
	//int dy = 1, dx = 1;
		int diff_x = x2 - x1, diff_y = y2 - y1;
		slope = (y2 - y1)/((double)x2 - (double)x1);

	// slope < abs(1)
		if (abs(slope) <= 1) {
			int x = x1;
			double y = (double)y1;
		//slope < 1 to right
			if (diff_x >= 0) {
				for (int i=0; i<diff_x; i++) {

				// std::cout << slope << std::endl;
				// std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;

					if (x > width || y > height) continue;
					dst.at<unsigned char>(static_cast<int>(y + 0.5), x) = l;

					x++;
					err += slope;
					if (diff_y >= 0 && err > 1/2) {
					// diff_y > 0  up right
						y = y+1;
						err = err - 1;
					} else if (diff_y < 0 && err < 1/2) {
					//diff_y < 0  down right
						y = y-1;
						err = err + 1;
					}

				}
			} else {
			// slope < 1 to left
				for (int i=0; i>diff_x; i--) {
					std::cout << slope << std::endl;
					std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;

					if (x > width || y > height) continue;

					dst.at<unsigned char>(static_cast<int>(y + 0.5), x) = l;

					x--;
					err -= slope;
					if (diff_y >= 0 && err > 1/2) {
					// up left
						y += 1;
						err -= 1;
					} else if (diff_y < 0 && err < 1/2) {
					// down left
						y -= 1;
						err += 1;
					}
				}
			}
		} else {
		// when slope > abs(1)
			int y = y1;
			double x = (double)x1;
			if (diff_y >= 0) {
			// slope > 1 up
				for (int i=0; i<diff_y; i++) {
					std::cout << slope << std::endl;
					std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;
					if (x > width || y > height) continue;

					dst.at<unsigned char>(static_cast<int>(y + 0.5), x) = l;
					y++;
					err += 1/slope;
					if (diff_x >= 0 && err > 1/2) {
					// up right
						x += 1;
						err -= 1;
					} else if (diff_x < 0 && err < 1/2) {
					//  up left
						x -= 1;
						err += 1;
					}
				}
			} else {
			//slope > 1 down
				for (int i=0; i>diff_y; i--) {
					std::cout << slope << std::endl;
					std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;
					if (x > width || y > height) continue;

					dst.at<unsigned char>(static_cast<int>(y + 0.5), x) = l;

					y--;
					err -= 1/slope;
					if (diff_x >= 0 && err > 1/2) {
					// up right
						x += 1;
						err -= 1;
					} else if (diff_x < 0 && err < 1/2) {
					// up left
						x -= 1;
						err += 1;
					}
				}
			}
		}
		cv::imwrite("line_bres.png", dst);
	}

	void draw_line(cv::Mat &dst, int x1, int y1, int x2, int y2, unsigned char l) {
		double slope = 0;
		int height = dst.rows;
		int width = dst.cols;
	//int dy = 1, dx = 1;
		int diff_x = x2 - x1, diff_y = y2 - y1;
		slope = (y2 - y1)/((double)x2 - (double)x1);

	// SLOPE SIZE < abs(1)
		if (abs(slope) <= 1) {
			int x = x1;
			double y = (double)y1;
		//slope < 1 to right
			if (diff_x >= 0) {

				for (int i=0; i<diff_x; i++) {

				// std::cout << slope << std::endl;
				// std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;

					if (x > width || y > height || x<0 || y<0) continue;
					dst.at<unsigned char>(static_cast<int>(y + 0.5), x) = l;
					x++;
					y += slope;
				}
			} else {
			// slope < 1 to left
				for (int i=0; i>diff_x; i--) {
				// std::cout << slope << std::endl;
				// std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;

					if (x > width || y > height || x<0 || y<0) continue;

					dst.at<unsigned char>(static_cast<int>(y + 0.5), x) = l;

					x--;
					y -= slope;
				}
			}
		} else {
		// when slope > abs(1)
			int y = y1;
			double x = (double)x1;
		// slope > 1 to right
			if (diff_y >= 0) {
				for (int i=0; i<diff_y; i++) {
				// std::cout << slope << std::endl;
				// std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;
					if (x > width || y > height || x<0 || y<0) continue;

					dst.at<unsigned char>(static_cast<int>(y + 0.5), x) = l;
					y++;
					x += 1/slope;
				}
			} else {
			//slope > 1 to left
				for (int i=0; i>diff_y; i--) {
				// std::cout << slope << std::endl;
				// std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;
					if (x > width || y > height || x<0 || y<0) continue;

					dst.at<unsigned char>(static_cast<int>(y + 0.5), x) = l;

					y--;
					x -= 1/slope;
				}
			}
		}
	}

	void draw_circle(cv::Mat& dst, int x0, int y0, unsigned r, unsigned char l) {
		int x, y;
		int height = dst.rows;
		int width = dst.cols;
		for (int theta=0; theta<2*3.14*r; theta++) {
			x = x0 + r*cos((double)theta/r);
			y = y0 + r*sin((double)theta/r);
			if (x > width || y > height) continue;

			std::cout << x << ' ' << y << std::endl;

			dst.at<unsigned char>(y, x) = l;
		}
	}
};
