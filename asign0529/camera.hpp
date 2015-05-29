#include <fstream>
#include <iostream>
#include "../asign0508/ppoint.hpp"
#include <opencv2/opencv.hpp>
#include "../asign0522/8_geometry4/skip_comments.hpp"
#include "../asign0522/skel.hpp"
#include "../asign0522/cg.hpp"
#include "../asign0522/transform_3d.hpp"

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
/*
		void Rotate(Matx44d &R0) {
			Matx44d T = _location;
			Matx33d R(T(0,0), T(0,1), T(0,2),
										T(1,0), T(1,1), T(1,2),
										T(2,0), T(2,1), T(2,2));
			Matx31d t(T(1,0), T(1,0), T(2,0));
			R = R0*R;
			t = R*t;
			_location = Matx44d(R(0,0), R(0,1), R(0,2), t(0,0),
																R(1,0), R(1,1), R(1,2), t(1,0),
																R(2,0), R(2,1), R(2,2), t(2,0),
																R(3,0), R(3,1), R(3,2), t(3,0));
		}*/
		/*
		void Translate(Matx44d &T0) {
			Matx44d T = _location;
			Matx33d R(T(0,0), T(0,1), T(0,2),
					T(1,0), T(1,1), T(1,2),
					T(2,0), T(2,1), T(2,2));
			Matx13d t(T(0,0), T(0,1), T(0,2));
			R = T0*R;
			t = R*t;
			_location = Matx44d(R(0,0), R(0,1), R(0,2), t(0,0),
					R(1,0), R(1,1), R(1,2), t(1,0),
					R(2,0), R(2,1), R(2,2), t(2,0),
					R(3,0), R(3,1), R(3,2), t(3,0));
		}
		*/
};


void draw_skel_perspective(Mat &img, const SKEL &skel, unsigned char gray);
void draw_skels_perspective(Mat& img, const vector<SKEL>& skels, const Camera& C, unsigned char gray);

void draw_skels_perspective(Mat& img, const vector<SKEL>& skels, const Camera& C, unsigned char gray) {
	// for skel in skels {
	Matx34d I0(1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0);
	Matx43d I1(1, 0, 0,
								0, 1, 0,
								0, 0, 1,
								0, 0, 0);
	Camera _c = C;
	for (unsigned i=0; i<skels.size(); i++) {
		Matx44d R = I1*_c.get_intrinsic()*I0*_c.get_location();
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
      if (hide_flag != 1) {
        double p1x, p1y;
        p1x = projected_v[lines[i][j]].x()*f + bias;
        p1y = projected_v[lines[i][j]].y()*f + bias;
        //cout << p1x << ' ' << p1y << endl;
        Point2f p1(p1x, p1y);
        Point2f p2(projected_v[lines[i][j+1]].x()*f + bias, projected_v[lines[i][j+1]].y()*f + bias);
        //line(img, projected_v[lines[i][j]], projected_v[lines[i][j+1]], 255);
        //cout << p1 << ' ' << p2 << endl;
        line(img, p1, p2, gray);
      }
      hide_flag = 0;
    }
  }
}
