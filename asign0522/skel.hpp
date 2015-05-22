#include <fstream>
#include <iostream>
#include "../asign0508/ppoint.hpp"
#include <opencv2/opencv.hpp>
#include "8_geometry4/skip_comments.hpp"

using namespace tutor;
using namespace std;
using namespace cv;

typedef std::vector<double> Polyline;

class SKEL{
	private:
		std::vector<PPoint3d> _vertices;
		std::vector<Polyline> _lines;
		cv::Matx44d _matrix;							// convert matrix
		SKEL();

	public:
		explicit SKEL(const std::string &fn) {
			string line;
			vector<double> vec;
			int v_num, p_num;
			string type;

			ifstream skel_file (fn.c_str());
			//ifstream skel_file ("cg.skel");
			//cout << skel_file << endl;
			skipComments(skel_file) >> type ;
			//skel_file.open(fn.c_str);
			skipComments(skel_file) >> v_num ;
			//cout << v_num;
			skipComments(skel_file) >> p_num ;
			// read vertices
			double x, y, z, w;
			for (int i=0; i<v_num; i++) {
				skipComments(skel_file) >> x >> y >> z >> w ;
				//cout << x << endl;
				PPoint3d p(x,y,z,w);
				_vertices.push_back(p);
			}
			// read polyline
			for (int i=0; i<p_num; i++) {
				skipComments(skel_file) >> x >> y >> z >> w ;
				Polyline p;
				p.push_back(x);
				p.push_back(y);
				p.push_back(z);
				p.push_back(w);
				_lines.push_back(p);
			}
		}

		void print_vertices() {
			for (int i=0; i<(int)_vertices.size(); i++) {
				cout << _vertices[i][0] << ' '
						<< _vertices[i][1] << ' '
						<< _vertices[i][2] << ' '
						<< _vertices[i][3] << ' '<< endl;
			}
		}
		void print_polylines() {
			for (int i=0; i<(int)_lines.size(); i++) {
				cout << _lines[i][0] << ' '
						<< _lines[i][1] << ' '
						<< _lines[i][2] << ' '
						<< _lines[i][3] << ' '<< endl;
			}
		}



		const std::vector<PPoint3d>& vertices() const{
			return _vertices;
		}
		const std::vector<Polyline>& polylines() const {
			return _lines;
		}

		void set_matrix(cv::Matx44d matrix) {
			_matrix = matrix;
		}
		void transform(Matx44d &T) {
			for (unsigned i=0; i<_vertices.size(); i++) {
				//cout << T(0,0) << endl;
				_vertices[i] = T*_vertices[i];
				//cout << _vertices[i][0] << endl;
			}
		}
};

