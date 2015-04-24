#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

//using namespace std;
void draw_line(cv::Mat&, int, int, int, int, unsigned char);
void draw_line_bres(cv::Mat&, int, int, int, int, unsigned char);
void draw_circle(cv::Mat&, int, int, unsigned, unsigned char);

int main() {
	int height = 320;
	int width = 480;
	int l = 50;
	int arr[10][4] = {
			{200, 200, 100, 300},
			{150, 200, 100, 300},
			{200, 250, 300, 300},
			{-50,  200, 50  , 100},
			{100, 50,   200,  -50},
			{-100, 150, 150, -100},
			{300, 350, 1500, 150},
			{300, 50, 450, 50},
			{400, 50, 400, 150}
	};

	cv::Mat dst(height, width, CV_8UC1, 255);

	for (int i = 0; i<10; i++) {
		//draw_line(dst, arr[i][0], arr[i][1], arr[i][2], arr[i][3], l);
		draw_line_bres(dst, arr[i][0], arr[i][1], arr[i][2], arr[i][3], l);

	}
	//draw_circle(dst, 200, 200, 180, l);

	//cv::imwrite("line_bres.png", dst);
	cv::imwrite("lines.png", dst);

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

				std::cout << slope << std::endl;
				std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;

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

				//y += slope;
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
					err += + 1;
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
				err += slope;
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
				err -= slope;
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

				std::cout << slope << std::endl;
				std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;

				if (x > width || y > height) continue;
				dst.at<unsigned char>(static_cast<int>(y + 0.5), x) = l;
				x++;
				y += slope;
			}
		} else {
			// slope < 1 to left
			for (int i=0; i>diff_x; i--) {
				std::cout << slope << std::endl;
				std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;

				if (x > width || y > height) continue;

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
				std::cout << slope << std::endl;
				std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;
				if (x > width || y > height) continue;

				dst.at<unsigned char>(static_cast<int>(y + 0.5), x) = l;
				y++;
				x += 1/slope;
			}
		} else {
			//slope > 1 to left
			for (int i=0; i>diff_y; i--) {
				std::cout << slope << std::endl;
				std::cout << x << ' ' << (int)y << ' ' << l <<  std::endl;
				if (x > width || y > height) continue;

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

