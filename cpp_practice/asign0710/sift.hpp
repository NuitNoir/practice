#include <stdio.h>
#include <climits>
#include <ctime>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


class SIFT {
public:
	cv::Mat_<unsigned char> src;
	cv::Mat_<unsigned char> img_char;
	cv::Mat_<unsigned char> img;
	cv::Mat_<double> img_d;

	cv::Mat_<double> img_fx2;
	cv::Mat_<double> img_fy2;
	cv::Mat_<double> img_fxy;

	cv::Mat_<double> mat_intensity;
	cv::Mat_<double> mat_radian;
	cv::Mat_<double> mat_weight;

	double sigma;

	std::string img_dir;
	std::string ext;
	std::string sig_str;

	SIFT(cv::Mat_<unsigned char> img) {
		this->img = img.clone();
		this->src = img.clone();
		this->img_char = img.clone();
		this->img.convertTo(this->img_d, CV_64FC1);	
		
		int rows = this->img.rows, cols = this->img.cols;
		this->mat_radian = cv::Mat::zeros(rows, cols, CV_64FC1);
		this->mat_weight = cv::Mat::zeros(rows, cols, CV_64FC1);

	}

	void set_img(cv::Mat_<unsigned char> img) {
		this->img = img.clone();
		this->img_char = img.clone();
		this->img.convertTo(this->img_d, CV_64FC1);
	}

	double get_intensity(int y, int x) {
		double intensity = 0;	
		intensity = sqrt(this->img_fx2(y, x) + this->img_fy2(y, x));
		// std::cout << "intensity=" << intensity << std::endl;
		return intensity;
	}
	cv::Mat_<double> get_intensity_mat() {
		int rows = this->img.rows, cols = this->img.cols;
		// cv::Mat_<double> mat_intensity(rows, cols);
		this->mat_intensity = cv::Mat::zeros(rows, cols, CV_64FC1);
		for (int y=0; y < rows; y++) {
			for (int x=0; x < cols; x++) {
				this->mat_intensity[y][x] = get_intensity(y, x);
			}
		}
		return this->mat_intensity;
	}

	double get_radian(int y, int x) {
		double radian = 0;
		double fx = sqrt(this->img_fx2(y,x));
		double fy = sqrt(this->img_fy2(y,x));
		radian = atan(fy/fx);
		if (fx == 0) radian = 2*M_PI + 1;
		// if (fx != 0 && fy != 0) std::cout << "radian=" << radian << " fx=" << fx << " fy=" << fy <<  std::endl;
		return radian;
	}
	cv::Mat_<double> get_radian_mat() {
		int rows = this->img.rows, cols = this->img.cols;
		for (int y=0; y < rows; y++) {
			for (int x=0; x < cols; x++) {
				this->mat_radian[y][x] = get_radian(y, x);
			}
		}
		return this->mat_radian;
	}

	cv::Mat_<double> get_weight_mat(double sigma) {
		int rows = this->img.rows, cols = this->img.cols;
		// cv::Mat_<double> mat_weight(rows, cols);
		for (int y=0; y < rows; y++) {
			for (int x=0; x < cols; x++) {
				this->mat_weight[y][x] = get_weight(y, x, sigma);
			}
		}
		return this->mat_weight;
	}
	double get_gauss_val(int y, int x, double sigma) {
		double gauss = (1.0/(sigma*sigma*2.0*M_PI))*exp(-x*x/(2.0*sigma*sigma) ) ;
		return gauss;
	}

	double get_weight(int y, int x, double sigma) {
		// this->differentiate();
		double intensity = this->get_intensity(y, x);
		double gauss_val = this->get_gauss_val(y, x, sigma);

		double weight = intensity*gauss_val;
		return weight;
	}

	cv::Mat_<double> get_affine_mat(double theta) {
		cv::Mat_<double> R = (cv::Mat_<double>(2, 2) << cos(theta), sin(theta),
						-sin(theta), cos(theta) );
		return R;
	}
	///// return 0...7
	int get_direction(cv::Mat_<unsigned char> img, int y, int x) {
		int direction = -1;
		return direction;
	}
	///// return major_direction 
	int get_major_direction(int y, int x, int N, cv::Mat_<double> radian_mat, cv::Mat_<double> intensity_mat) {
		int hist_dim = 36;
		double hist[hist_dim] ;
		sift.differentiate(this->src);
		cv::Mat_<double> intensity_mat = sift.get_intensity_mat();
		cv::Mat_<double> radian_mat = sift.get_radian_mat();
		std::fill_n(hist, hist_dim, 0);
		for (int i= -N; i<= N; i++) {
			for (int j= -N; j<= N; j++) {
				///// TODO theta must be 0-360 but now 0-90. 
				double theta = this->get_img_val( y+i, x+j, radian_mat);
				// comment_timestamp("theta=" + std::to_string(theta) + ' ' + std::to_string(j));
				int direction = theta*(hist_dim/(2*M_PI)); ///// direction [0, 35]
				if (theta == 2*M_PI+1) continue;
				// if ( !(direction>=0 && direction<hist_dim)) assert("direction overflow" + std::to_string(direction));
				assert( (direction>=-1 && direction < hist_dim ));
				double weight = this->get_img_val(y+i, x+j, intensity_mat) * this->get_gauss_val(i,j,sigma);
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
		return major_direction;
	}

	cv::Mat_<double> get_sift_descriptor() {
				//////////////////////////////////////////////////
		///// sift descriptor
		double theta = (-1)*major_direction*(2*M_PI/36);
		cv::Mat_<unsigned char> dst = sift.rotate(theta, y, x);
		sift.differentiate();
		cv::Mat_<double> intensity_mat = sift.get_intensity_mat();
		cv::Mat_<double> radian_mat = sift.get_radian_mat();

		int grid_base = 4;
		int grid_mul = N/grid_base + 1;
		int grid_size = grid_base*grid_mul;

		int hist_dim = 8;
		/////  descriptor[i][j] has direction (0...7)  grid_size x grid_size
		cv::Mat_<unsigned char> descriptor = cv::Mat::zeros(grid_size, grid_size, CV_8UC1);
		///// 4x4 8channel 
		cv::Mat_<double> sift_descriptor = cv::Mat::zeros(grid_base*grid_base*hist_dim, 1, CV_8UC1);
	
		for (int i=-grid_size/2; i<grid_size/2; i++) {
			for (int j=-grid_size/2; j<grid_size/2; j++) {
				///// TODO theta must be 0-360 but now 0-90. 
				double theta = sift.get_img_val( y+i, x+j, radian_mat);
				int direction = theta*(hist_dim/(2*M_PI)); ///// direction [0, 8]
				if (theta == 2*M_PI+1) continue;
				assert( (direction>=-1 && direction < hist_dim ));
				double weight = sift.get_img_val(y+i, x+j, intensity_mat) * sift.get_gauss_val(i,j,sigma);

				descriptors(i+grid_size/2, j+grid_size/2) = sift.get_direction(dst, y+i, x+j);
				// descriptor[i][j] = sift.get_direction((int)(pt1.y + i*dy + 0.5), (int)(pt1.x + i*dx + 0.5) ); ///// get image value by Nearest Neightbor. 
				int grid_row = i/grid_mul, grid_col = j/grid_mul;
				sift_descriptor(grid_row*grid_base*hist_dim + grid_col*hist_dim + descriptor(i,j), 0) += weight;
			}
		}
		std::cout << sift_descriptor << std::endl;
		//////////////////////////////////////////////////
		sift_descriptors.push_back(sift_descriptor);
		return sift_descriptor;
	}

	cv::Mat_<unsigned char> rotate(double angle, int y, int x) {
	    	int rows = this->src.rows, cols = this->src.cols;

	    	cv::Point2f pt(y, x);
	    	cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);

	    	cv::warpAffine(this->src, this->img, r, cv::Size(rows, cols));
	    	return this->img;
	}


	// get image differentiation 
	// fx^2, fxfy, fyfx, fy^2
	std::vector<cv::Mat_<double> > differentiate() {
		std::vector<cv::Mat_<double>  > imgs;
		imgs = this->differentiate(this->img);
		return imgs;
	}
	std::vector<cv::Mat_<double> > differentiate(cv::Mat_<unsigned char> &img) {
		std::vector<cv::Mat_<double> > imgs;
		int cols = img.cols;
		int rows = img.rows;
		this->img_fx2 = cv::Mat::zeros(rows, cols, CV_64FC1);
		this->img_fy2 = cv::Mat::zeros(rows, cols, CV_64FC1);
		this->img_fxy = cv::Mat::zeros(rows, cols, CV_64FC1);

		// cv::Mat_<double> mat_fx2(rows, cols);
		// cv::Mat_<double> mat_fy2(rows, cols);
		// cv::Mat_<double> mat_fxy(rows, cols);
		std::cout << "rows=" << rows << "  cols="  << cols << std::endl;
		double fx, fy;
		for (int y=0; y<rows; y++) {
			for (int x=0; x<cols; x++) {
				fx = (get_img_val(y, x-1, img) - get_img_val(y, x+1, img) ) / 2; 
				fy = (get_img_val(y-1, x, img) - get_img_val(y+1, x, img) ) / 2; 

				this->img_fx2(y, x) = fx*fx;
				this->img_fy2(y, x) = fy*fy;
				this->img_fxy(y, x) = fx*fy;
			}
		}
		cv::imwrite(this->img_dir + "fx2" + this->ext, img_fx2);
		cv::imwrite(this->img_dir + "fy2" + this->ext, img_fy2);
		cv::imwrite(this->img_dir + "fxy" + this->ext, img_fxy);

		imgs.push_back(this->img_fx2);
		imgs.push_back(this->img_fy2);
		imgs.push_back(this->img_fxy);
		comment_timestamp("differentiate end" );
		return imgs;
	}

	double get_img_val(int y, int x, cv::Mat_<double>  img) {
		int cols = img.cols;	
		int rows = this->img.rows;
		if (x < 0) x= -x-1;
		if (x >= cols) x =  (cols-1) - (x - (cols-1));
		if (y < 0) y= -y-1;
		if (y >= rows) y =  (rows-1) - (y - (rows-1)) ;
		double val = img(y, x);
		return val;
	}
	void set_sigma(double sigma) {
		std::stringstream ss;
		ss << sigma;
		this->sig_str = ss.str();
	}

};