#include <opencv2/opencv.hpp>
#include <sstream>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <opencv2/highgui/highgui.hpp>

void GetPosition(int event, int x, int y, int flag, void*) ;

//  get image pixel position  which is clicked.
int main(int argc, char* argv[]) {
	std::string w1_name = "sample";

	// cv::Mat img(500, 500, CV_8UC1, 255);
	cv::Mat img = cv::imread(argv[1], 0);
	cv::namedWindow("sample",   CV_WINDOW_NORMAL);//CV_WINDOW_AUTOSIZE);
	cv::setMouseCallback("sample", &GetPosition);
	cv::imshow("sample", img);
	while(1) {
		int key = cv::waitKey(0);
		if(key) break;
	}
	return 1;
}

void GetPosition(int event, int x, int y, int flags, void*) {
	std::string event_description;

	// マウスイベントを取得
	switch(event) {
	case cv::EVENT_MOUSEMOVE:
		event_description += "MOUSE_MOVE";
		break;
	case cv::EVENT_LBUTTONDOWN:
		event_description += "LBUTTON_DOWN";
		break;
	case cv::EVENT_RBUTTONDOWN:
		event_description += "RBUTTON_DOWN";
		break;
	case cv::EVENT_MBUTTONDOWN:
		event_description += "MBUTTON_DOWN";
		break;
	case cv::EVENT_LBUTTONUP:
		event_description += "LBUTTON_UP";
		break;
	case cv::EVENT_RBUTTONUP:
		event_description += "RBUTTON_UP";
		break;
	case cv::EVENT_MBUTTONUP:
		event_description += "MBUTTON_UP";
		break;
	case cv::EVENT_LBUTTONDBLCLK:
		event_description += "LBUTTON_DBLCLK";
		break;
	case cv::EVENT_RBUTTONDBLCLK:
		event_description += "RBUTTON_DBLCLK";
		break;
	case cv::EVENT_MBUTTONDBLCLK:
		event_description += "MBUTTON_DBLCLK";
		break;
	}

	// マウスボタン，及び修飾キーを取得
	if(flags & cv::EVENT_FLAG_LBUTTON)
		event_description += " + LBUTTON";
	if(flags & cv::EVENT_FLAG_RBUTTON)
		event_description += " + RBUTTON";
	if(flags & cv::EVENT_FLAG_MBUTTON)
		event_description += " + MBUTTON";
	if(flags & cv::EVENT_FLAG_CTRLKEY)
		event_description += " + CTRL";
	if(flags & cv::EVENT_FLAG_SHIFTKEY)
		event_description += " + SHIFT";
	if(flags & cv::EVENT_FLAG_ALTKEY)
		event_description += " + ALT";

	if (event_description == "LBUTTON_DOWN") {
		std::cout <<  x <<  " " << y <<  std::endl;
	}
}

