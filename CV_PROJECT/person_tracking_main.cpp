#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <list>
#include "Header/Component.h"
#include "Header/ConnectedComponent.h"
#include "Header/Logic.h"
#include "Header/BgDiff.h"
#include "Header/personID.h"

#define INIT_SKIP_FRAME 20
#define WHITE Scalar(255,255,255)
#define RED Scalar (0,0,255)
using namespace cv;
using namespace std;

vector<Component> personList;
vector<Component> nonpersonList;
int thresholdCM = 50;
string imgNum = "6";
RNG rng(12345);

int main() {
	cout << "start...\n";
	VideoCapture cap("IMG" + imgNum + ".mp4");
	//VideoCapture cap(0);
	if ( !cap.isOpened() ) {
		cout << "video error\n";
		return -1;
	}
	Mat f, diffBool, bg, foreground;
	do { cap >> f; } while ( f.empty() );
	//for ( int i = 0; i < INIT_SKIP_FRAME; i++ ) cap >> f;
	cap >> bg;
	bool loop = true, showPath = true;
	while ( loop ) {
		cap >> f;
		if ( f.empty() ) break;
		imshow("background", bg);
		bgSubtract(bg, f, diffBool, foreground);
		vector<Component> newFrameComponent;
		findComponentContour(diffBool, newFrameComponent, foreground);
		//cout << "component num: " << newFrameComponent.size() << "\n";

		//show rectangle of each component
		/*
		Mat connectedComponent = Mat::zeros(diffBool.rows, diffBool.cols, CV_8U);
		for (int i = 0; i < newFrameComponent.size(); i++) {
		for (int j = newFrameComponent[i].rect_tl.x; j < newFrameComponent[i].rect_br.x; j++) {
		for (int k = newFrameComponent[i].rect_tl.y; k < newFrameComponent[i].rect_br.y; k++) {
		connectedComponent.at<uchar>(j, k) = 255;
		}
		}
		}
		imshow("ConnectedComponent", connectedComponent);
		*/

		updateComponent(newFrameComponent, personList, nonpersonList, thresholdCM);
		cout << "person: " << personList.size() << " nonperson: " << nonpersonList.size() << "\n";
		if ( !personList.empty() ) {
			cout << "personID: ";
			for ( int i = 0; i < personList.size(); i++ ) {
				cout << personList[i].id << " ";
			}
			cout << "\n";
		}
		if ( showPath ) {
#pragma omp parallel for
			for ( int i = 0; i < personList.size(); i++ ) {
				for ( int j = 1; j < personList[i].path.size(); j++ ) {
					line(foreground, personList[i].path[j], personList[i].path[j - 1], RED, 1, 8, 0);
				}
			}
		}
		imshow("foreground", foreground);
		switch ( waitKey(10) ) {
			case 27: loop = false; break;
			case 'p': showPath = !showPath; break;
		}
	}
	cout << "end video\n";
	//getchar();
}