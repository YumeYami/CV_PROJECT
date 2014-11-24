#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <list>
#include "Component.h"
#include "ComponentX.h"
#include "bgSubtract.h"
#include "extractor.h"
#include "logic.h"
#include "draw.h"

#define INIT_SKIP_FRAME 20
#define WHITE Scalar(255,255,255)

using namespace cv;
using namespace std;
///old version
vector<Component> personList;
vector<Component> nonpersonList;
///new version
vector<ComponentX> componentX;
vector<Person> personListX;
vector<Item> itemListX;
vector<Item> unknowListX;

int thresholdCM = 40;
string imgNum = "4";
RNG rng(12345);
int posX, posY;
bool click = false;
bool bghsv = false;
void onMouse(int e, int x, int y, int fl, void*) {
	if ( e == EVENT_LBUTTONDOWN ) {
		posX = x;
		posY = y;
		click = true;
	}
}
int main() {
	cout << "start...\n";
	//VideoCapture cap("video/IMG" + imgNum + ".mp4");
	VideoCapture cap("video/video0" + imgNum + ".mp4");
	//VideoCapture cap(0);
	if ( !cap.isOpened() ) {
		cout << "video error\n";
		return -1;
	}
	namedWindow("foreground");
	setMouseCallback("foreground", onMouse);
	Mat f, diffBool, bg, foreground;
	///skip empty frame
	do { cap >> f; } while ( f.empty() );
	///skip initial frame
	//for ( int i = 0; i < INIT_SKIP_FRAME; i++ ) cap >> f;
	cap >> bg;
	bool loop = true, pause = false;
	while ( loop ) {
		if ( !pause ) cap >> f;
		if ( f.empty() ) break;
		imshow("background", f);
		if ( !bghsv ) {
			bgSubtract(bg, f, diffBool, foreground);
		}
		else {
			bgSubtractHSV(bg, f, diffBool, foreground);
		}
		///old version
		vector<Component> newFrameComponent;
		findComponentContour(diffBool, newFrameComponent, foreground);
		updateComponent(newFrameComponent, personList, nonpersonList, thresholdCM);
		///new version
		// 		vector<ComponentX> newComponentX;
		// 		findComponentXContour(diffBool, newComponentX, foreground);
		// 		updateComponentX(newComponentX, componentX, personListX, itemListX);
		// 		///

		drawComponents(foreground, newFrameComponent);
		drawPersonPath(foreground, personList, posX, posY, click);
		drawTextStatus(foreground, personList, nonpersonList);
		click = false;
		imshow("foreground", foreground);

		switch ( waitKey(10) ) {
			case 27: loop = false; break;
			case 'p': pause = !pause; break;
			case 'b': 
				bghsv = !bghsv;
				cout << bghsv;
				break;
		}
	}
	cout << "end video\n";
	//getchar();
}