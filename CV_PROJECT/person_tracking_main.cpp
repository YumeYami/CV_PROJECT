#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <list>
#include "Component.h"
#include "ConnectedComponent.h"
#include "Logic.h"
#include "BgDiff.h"
#include "personID.h"

#define INIT_SKIP_FRAME 20


using namespace cv;
using namespace std;

vector<Component> personList;
vector<Component> nonpersonList;

int thresholdCM = 10;


int main() {
	cout << "start...\n";
	VideoCapture cap(0);
	if ( !cap.isOpened() ) {
		cout << "video error\n";
		return -1;
	}
	Mat f, diffBool, bg;
	do { cap >> f; } while ( f.empty() );
	for ( int i = 0; i < INIT_SKIP_FRAME; i++ ) cap >> f;
	cap >> bg;

	bool loop = true;
	while ( loop ) {
		cap >> f;
		if ( f.empty() ) break;
		imshow("background", bg);
		bgSubtract(bg, f, diffBool);
		vector<Component> newFrameComponent;

		findComponent(diffBool, newFrameComponent);
		// show rectangle of each component
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

		updateComponent(newFrameComponent, personList, nonpersonList,thresholdCM);

		switch (waitKey(100)) {
			case 27: loop = false; break;
		}
	}
	cout << "end video\n";
	//getchar();
}