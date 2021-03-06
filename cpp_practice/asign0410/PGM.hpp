/*
 * PGM.hpp
 *
 *  Created on: 2015/04/10
 *      Author: kyota
 */

#ifndef PGM_HPP
#define PGM_HPP

#include <cstdlib>
#include <valarray>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

namespace PNM{
std::string get_token(std::istream &ifs) {
	std::string token;
	ifs >> token;
	//skip comment
	while(token[0] == '#') {
		while (ifs.get() != '\n'); //empty loop
		ifs >> token;
	}
	return token;
}


class PGM{
private:
	std::string magick_num;
	unsigned width;
	unsigned height;
	std::string max;
	std::valarray <unsigned char> data;
	std::valarray <unsigned char> inv_data;
	std::valarray <unsigned char> mono_data;
	std::valarray <unsigned char> rot90_data;
private:
	   //forbid default constractor
public:
	//constructors
	PGM(unsigned w, unsigned h)
: width(w), height(h), data(w*h)
{}

	//ファイル名fnからファイルを取得し、書き込む。
	explicit PGM(const std::string &fn) {
		std::ifstream ifs(fn.c_str(), std::ios::binary);
		if(ifs.fail()) {
			std::cerr << "file open error" << std::endl;
			std::abort();
		}

		//read PGM header and data
		//write myself code
		magick_num = get_token(ifs);
		width = std::atoi(get_token(ifs).c_str());
		height = std::atoi(get_token(ifs).c_str());
		if (magick_num != "P1" && magick_num != "P3") {
			max = get_token(ifs);
		}

		unsigned char *buf = new unsigned char[width*height];
		//data = new unsigned char[width*height];
		data.resize(width*height, 0	);
		for (unsigned i=0; i < width*height; i++) {
			ifs.read( (char *)buf, 1);
			data[i] =	*buf;
			//std::cout << data[i];
		}
		ifs.close();

		//デバッグ用の値出力
		std::cout << magick_num << std::endl;
		std::cout << width << std::endl;
		std::cout << height << std::endl;
		std::cout << max << std::endl;
	}

	PGM(const PGM &p)
	: width(p.width), height(p.height), data(p.data)
	{}

	//destructor
	~PGM(){}

	//write PGM data
	void write(const std::string &fn)const
	{
		std::ofstream outfile("copy_cat1.pgm");

		outfile << magick_num << std::endl << width << ' ' << height << std::endl << max ;
		for (unsigned i=0; i<data.size(); i++) {
			outfile.write( (char *)&data[i], 1);
		}
	}
	//invert gray scale
	void invert(const std::string &fn)
	{
		std::ofstream ofs("invert_cat1.pgm");
		inv_data.resize(width*height, 0);

		ofs << magick_num << std::endl << width << ' ' << height << std::endl << max << std::endl;
		for (unsigned i=0; i<data.size(); i++) {
			std::cout << (255-data[i]) << std::endl;
			inv_data[i] = (255 - data[i]);
			//std::cout << inv_data[i];
		}

		for (unsigned i=0; i<data.size(); i++) {
			ofs.write( (char *)&inv_data[i], 1);
		}
	}

	//monochrome image
	void monochrome(const std::string &fn)
	{
		std::ofstream ofs("monochrome_cat1.pgm");
		mono_data.resize(width*height, 0);

		ofs << magick_num << std::endl << width << ' ' << height << std::endl << max << std::endl;
		for (unsigned i=0; i<data.size(); i++) {
			//std::cout << (255-data[i]) << std::endl;
			mono_data[i] = data[i]>(255/2) ? 255 : 0;
		}

		for (unsigned i=0; i<data.size(); i++) {
			ofs.write( (char *)&mono_data[i], 1);
		}
	}
	//monochrome image
	void rotate(const std::string &fn)
	{
		std::ofstream ofs("rot90_cat1.pgm");
		rot90_data.resize(width*height, 0);


		unsigned x, y;
		ofs << magick_num << std::endl << height << ' ' << width << std::endl << max << std::endl;
		for (unsigned i=0; i<data.size(); i++) {
			//std::cout << (255-data[i]) << std::endl;
			//i = y*width + x -> x*width + y
			//  x = width - y, y = x
			x = i%width;
			y = i/width;
			rot90_data[i] = data[x*height + (width-y)];
		}

		for (unsigned i=0; i<data.size(); i++) {
			ofs.write( (char *)&rot90_data[i], 1);
		}
	}


	unsigned getWidth() const {return width; }
	unsigned getHeight() const { return height; }

	//access operators
	const unsigned char& operator()(int x, int y) const {
		assert( 0 <= x ); assert( x < static_cast<int>(width));
		assert( 0 <= y ); assert( y < static_cast<int>(height));

		return data[width*y + x];
	}

	unsigned char& operator()(int x, int y){
		assert( 0 <= x ); assert( x < static_cast<int>(width) );
		assert( 0 <= y ); assert( y < static_cast<int>(height));

		return data[width*y + x];
	}
}; // end of class PGM
} // end of namespace PNM



#endif /* PGM_HPP_ */
