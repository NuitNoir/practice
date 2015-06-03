#include <fstream>
#include <iostream>
#include "../asign0508/ppoint.hpp"
#include <opencv2/opencv.hpp>
#include "../asign0522/8_geometry4/skip_comments.hpp"
#include "../asign0522/skel.hpp"
// #include "../asign0522/cg.hpp"
#include "../asign0522/transform_3d.hpp"
#include "../asign0423/drawLine.hpp"

using namespace tutor;
using namespace std;
using namespace cv;

class Camera {
private:
	unsigned _width, _height;
		Matx33d _intrinsic;  // inner parameter
		Matx44d _location;		//	location and derection

	public:
		Matx33d get_intrinsic() {
			return _intrinsic;
		}
		Matx44d get_location() {
			return _location;
		}
		void set_size(unsigned width, unsigned height) {
			_width = width;
			_height = height;
		}
		Matx33d set_intrinsic(double f, double ku, double kv, double s, double u0, double v0) {
			return _intrinsic = Matx33d(f*ku, f*s, u0,
				0, f*kv, v0,
				0, 0, 1);
		}
		void set_location(Matx44d M) {
			_location = M;
		}
		void transform(Matx44d &T) {
			_location = T*_location;
		}
		void draw_skels_perspective(Mat& img, const vector<SKEL>& skels, unsigned char gray) {
			Matx34d I0(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0);
			Matx43d I1(1, 0, 0,
				0, 1, 0,
				0, 0, 1,
				0, 0, 0);
			// Camera _c = this;
			for (unsigned i=0; i<skels.size(); i++) {
				Matx44d R = I1*this->get_intrinsic()*I0*this->get_location();
				SKEL _skel = skels[i];
				_skel.transform(R);
				draw_skel_perspective(img, _skel, gray);
			}
		}

		void draw_skel_perspective(Mat &img, const SKEL &skel, unsigned char gray) {
		  //skel.print_vertices();
			vector<PPoint3d> vertices = skel.vertices();
			vector<PPoint2d> projected_v;

			for (unsigned i=0; i < vertices.size(); i++) {
		    //cout << vertices[i][0] << endl;
				projected_v.push_back( perspective_projection(vertices[i]) );
			}

			int hide_flag = 0;
		  //skel.print_polylines();
			vector<Polyline> lines = skel.polylines();
			for (unsigned i=0; i<lines.size(); i++) {
		    for (unsigned j=1; j<lines[i][0]; j++) { //lines[i][0] has passing point num.
		      //draw_line(img, projected_v[lines[i][j]][0], projected_v[lines[i][j]][1],
		      //		projected_v[lines[i][j+1]][0], projected_v[lines[i][j+1]][1], 255);
		    	double f = 4, bias = 250;

		      //cout << lines[i][1] << lines[i][2] << lines[i][3] << endl;
		    	if (lines[i][0] >= 3) {
		        //hide_flag = hide_line(projected_v[lines[i][1]], projected_v[lines[i][2]], projected_v[lines[i][3]] );
		        //cout << hide_flag << endl;
		    	}
		      //if ( (projected_v[lines[i][j]][2] > EPS || projected_v[lines[i][j+1]][2] > EPS) && hide_flag != 1) {
		    	if (hide_flag == 1) continue;
 	    		double p1x, p1y, p1z;
	    		p1x = projected_v[lines[i][j]].x()*f + bias;
	    		p1y = projected_v[lines[i][j]].y()*f + bias;
	    		p1z = projected_v[lines[i][j]][2];
	    		double p2x, p2y, p2z;
	    		p2x = projected_v[lines[i][j+1]].x()*f + bias;
	    		p2y = projected_v[lines[i][j+1]].y()*f + bias;
	    		p2z = projected_v[lines[i][j]][2];
	        //cout << p1x << ' ' << p1y << endl;
	    		Point2f p1(p1x, p1y);
	    		Point2f p2(p2x, p2y);
	        // line(img, projected_v[lines[i][j]], projected_v[lines[i][j+1]], 255);
	        // cout << p1 << ' ' << p2 << endl;
	        // cout << p1z << ' ' << p2z << endl;
	        cout << img.cols << _location(2,3)<<' '<<p1z<<' '<<p2z << endl; 
	        double z = _location(2,3);
	        if (p1z < z && p2z < z) continue;
	        if ((p1x > img.cols || p1x < 0 || p1y > img.rows || p1y < 0) && 
	        	(p2x > img.cols || p2x < 0 || p2y > img.rows || p2y < 0)) {
	        	continue;
	        }	        	
	        // if (p2x > img.cols || p2x < 0 || p2y > img.rows || p2y < 0) continue;
    			// line(img, p1, p2, gray);	
    			draw_line(img, p1x, p1y, p2x, p2y, gray);
		    	hide_flag = 0;
		    }
		  }
	}
	PPoint2d perspective_projection(const PPoint3d &p3d) {
	  double f = 30;
	  //cout << p3d[0] << endl;
	  Matx34d M(f, 0, 0, 0,
	      0,f, 0, 0,
	      0, 0, 1, 0);
	  PPoint2d p2d = M*p3d;
	  return p2d;
	}
};