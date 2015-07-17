#include <stdio.h>
#include <climits>
#include <ctime>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "comment_timestamp.hpp"
#include "../lib/ppoint.hpp"
#include "../lib/affin.hpp"		
#include "sift.hpp"


int main() {
	cv::Mat_<unsigned char> img = cv::imread("img/Chessboard.png", 0);
	SIFT sift = SIFT(img);
	sift.img_dir = "img/";
	sift.ext = ".png";
	cv::Mat_<double> img_d = cv::imread(sift.img_dir + "Chessboard" + sift.ext, 0);

	sift.set_img(img);
	comment_timestamp(sift.img_dir + "Chessboard " + sift.ext);

	std::vector<cv::Mat_<double> > diff_imgs;

	int rows = sift.img.rows, cols = sift.img.cols;
	double k = 1.2;
	double sigma = 1 + k;

	std::vector<cv::Mat_<unsigned char> > src_imgs;
	std::vector<cv::Mat_<unsigned char> > feature_imgs;
	
	std::vector<cv::Mat_<double> > intensity_mats;
	std::vector<cv::Mat_<double> > radian_mats;
	std::vector<cv::Mat_<double> > weight_mats;

	///// read images
	for (int i= 0; i < 5; i++,  sigma += k) {
		sift.set_sigma(sigma);
		int N = sigma*3;
		cv::Mat_<unsigned char> feature_img =  cv::imread(sift.img_dir + "harris_laplacian1_" + sift.sig_str + sift.ext, 0);
		cv::Mat_<unsigned char> src_img = cv::imread(sift.img_dir + "gaussian" + sift.sig_str + sift.ext, 0);
		src_imgs.push_back(src_img);
		feature_imgs.push_back(feature_img);
	}
	comment_timestamp("image read end");
	///// intensity, radian, weight calculation
	sigma = 1;
	for (int i=0; i < (int)src_imgs.size(); i++, sigma+=k) {
		sift.set_img(src_imgs[i]);
		sift.set_sigma(sigma);
		// sift.differentiate(src_imgs[i]);
		cv::Mat_<double> img_fx2 = cv::imread(sift.img_dir + "fx2" + sift.sig_str + sift.ext, 0);
		cv::Mat_<double> img_fy2 = cv::imread(sift.img_dir + "fy2" + sift.sig_str + sift.ext, 0);
		comment_timestamp(sift.img_dir + "fx2" + sift.sig_str + sift.ext);
		sift.img_fx2 = img_fx2;
		sift.img_fy2 = img_fy2;
		cv::Mat_<double> intensity_mat = sift.get_intensity_mat();
		cv::Mat_<double> radian_mat = sift.get_radian_mat();
		// std::cout << radian_mat << std::endl;
		// weight_mat = 	sift.get_weight_mat(sigma);
		cv::imwrite(sift.img_dir + "intensity" + sift.sig_str + sift.ext, intensity_mat ); 
		cv::imwrite(sift.img_dir + "radian" + sift.sig_str + sift.ext, intensity_mat ); 
		intensity_mats.push_back(intensity_mat);
		radian_mats.push_back(radian_mat);
		// cv::imwrite(sift.img_dir + "weight" + sift.sig_str + sift.ext, intensity_mat ); 
	}
	comment_timestamp("intensity, radian, weight calculation end");
	/// gauss window
	sigma = 1;
	int hist_dim = 36;
	std::vector<cv::Mat_<unsigned char> > direction_mats;
	for (int scale=0; scale<(int)feature_imgs.size(); scale++, sigma+=k) {
		sift.set_sigma(sigma);
		// radian_mat = cv::imread(sift.img_dir + "radian" + sift.sig_str + sift.ext);
		// radian_mat = cv::imread(sift.img_dir + "weight" + sift.sig_str + sift.ext);
		int N = sigma*3;
		int nth = 0;
		for (int y=0; y < rows; y++) {
			for (int x=0; x < cols; x++) {
				if (feature_imgs[scale](y, x) == UCHAR_MAX) {
					nth ++;
					cv::Mat_<unsigned char> direction_mat = cv::Mat::zeros(rows, cols, CV_8UC1);
					// comment_timestamp("scale="+std::to_string(scale)+' '+std::to_string(nth) + "th feature point");
					// cv::Mat_<unsigned char> hist = cv::Mat::zeros(2*N, 2*N, CV_64FC1);
					double hist[hist_dim] ;
					std::fill_n(hist, hist_dim, 0);
					for (int i= -N; i<= N; i++) {
						for (int j= -N; j<= N; j++) {
							///// TODO theta must be 0-360 but now 0-90. 
							double theta = sift.get_img_val( y+i, x+j, radian_mats[scale]);
							// comment_timestamp("theta=" + std::to_string(theta) + ' ' + std::to_string(j));
							int direction = theta*(hist_dim/(2*M_PI)); ///// direction [0, 35]
							if (theta == 2*M_PI+1) continue;
							// if ( !(direction>=0 && direction<hist_dim)) assert("direction overflow" + std::to_string(direction));
							assert( (direction>=-1 && direction < hist_dim ));
							double weight = sift.get_img_val(y+i, x+j, intensity_mats[scale]) * sift.get_gauss_val(i,j,sigma);
							// comment_timestamp(std::to_string(i) + ' ' + std::to_string(j) + ' ' + std::to_string(direction));
							// std::cout << "weight=" <<  weight << "   \tdirection=" << direction<< std::endl;
							hist[direction] += weight;
							// std::cout << "direction=" << direction << " weight=" << hist[direction] << std::endl;
							// direction_mat[y+i][x+j] = direction*7;
						}
					}
					double max = -1;
					int major_direction = -1;
					for (int dir=0; dir<hist_dim; dir++) {
						// std::cout << hist[dir] << std::endl;
						if (hist[dir] > max) {
							max = hist[dir];
							major_direction = dir;
						}
						// std::cout << major_direction << ' ' << hist[dir] << std::endl;
					}
					///// TODO major direction is not only one. 
					std::cout << "major direction = " <<  major_direction << " hist val=" << max << std::endl;
					// cv::imwrite(sift.img_dir + "direction" + sift.sig_str + "_"+std::to_string(nth) + sift.ext, direction_mat);

					///// sift descriptor
					double theta = (-1)*major_direction*(2*M_PI/36);
					cv::Mat_<unsigned char> dst = sift.rotate(theta, y, x);
					sift.differentiate();
					cv::Mat_<double> intensity_mat = sift.get_intensity_mat();
					cv::Mat_<double> radian_mat = sift.get_radian_mat();

					int grid_base = 4
					int grid_mul = N/grid_base;
					int grid_size = grid_base*grid_mul ;
					if (grid_size == 0) grid_size = grid_base;
					/////  descriptor[i][j] has direction (0...7) 
					cv::Mat_<unsigned char> descriptor = cv::Mat::zeros(grid_size, grid_size, CV_8UC1);
					hist_dim = 8;
					for (int i=grid_size/2; i<grid_size/2; i++) {
						for (int j=grid_size/2; j<grid_size/2; j++) {

							///// TODO theta must be 0-360 but now 0-90. 
							double theta = sift.get_img_val( y+i, x+j, radian_mat);
							int direction = theta*(hist_dim/(2*M_PI)); ///// direction [0, 8]
							if (theta == 2*M_PI+1) continue;
							assert( (direction>=-1 && direction < hist_dim ));
							double weight = sift.get_img_val(y+i, x+j, intensity_mat) * sift.get_gauss_val(i,j,sigma);
							hist[direction] += weight;

							descriptor[i][j] = sift.get_direction(dst, y+i, x+j);
							// descriptor[i][j] = sift.get_direction((int)(pt1.y + i*dy + 0.5), (int)(pt1.x + i*dx + 0.5) ); ///// get image value by Nearest Neightbor. 
						}
					}
					for (int i=0)

				}
			}
		}
	}
	///// weight
	double weights[rows][cols];
	for (int y=0; y<rows; y++) {
		for (int x=0; x<cols; x++) {
			weights[y][x] = sift.get_weight(y, x, sigma);
		}
	}
	//// hist
	// sift.get_hist();

	return 0;
}
