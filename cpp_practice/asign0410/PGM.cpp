/*
 * PGM.cpp
 *
 *  Created on: 2015/04/10
 *      Author: kyota
 */

#include <iostream>
#include "PGM.hpp"

using namespace std;

int main() {
	//pgm = new PNM::PGM();
	//PNM::get_token("cat1.pgm");
	//std::string filename;
	//cin >> filename;
	//PNM::get_token(cin);


	PNM::PGM pgm("cat/cat1.pgm");
	//pgm.write("copy_cat1.pgm");
	//pgm.invert("invert_cat1.pgm");
	//pgm.monochrome("mono_cat1.pgm");
	pgm.rotate("rot90.pgm");
}

