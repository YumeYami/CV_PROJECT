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

string videoNum = "videox1.mp4";
 //string videoNum = "videox2.mp4";
 //string videoNum = "videox3.mp4";

#define INIT_SKIP_FRAME 15
#define WHITE Scalar(255,255,255)

using namespace cv;
using namespace std;
///old version
vector<Component> personList;
vector<Component> nonpersonList;
vector<Component> groupList;
///new version
vector<ComponentX> componentX;
vector<Person> personListX;
vector<Item> itemListX;
vector<Item> unknowListX;

int thresholdCM = 40;
int frameWidth;
int frameHeight;
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
	VideoCapture cap(videoNum);
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
	for ( int i = 0; i < INIT_SKIP_FRAME; i++ ) cap >> f;
	cap >> bg;
	setVideoSize(bg.cols, bg.rows);
	bool loop = true, pause = false, eachframe = false;
	while ( loop ) {
		vector<Component> newFrameComponent;
		if ( !pause ) {
			cap >> f;
			if ( f.empty() ) break;
			//imshow("background", f);
			if ( !bghsv ) {
				bgSubtract(bg, f, diffBool, foreground);
			}
			else {
				bgSubtractHSV(bg, f, diffBool, foreground);
			}
			///old version
			
			findComponentContour(diffBool, newFrameComponent, foreground);
			updateComponent(newFrameComponent, personList, nonpersonList, groupList, thresholdCM);
			///new version
			// 		vector<ComponentX> newComponentX;
			// 		findComponentXContour(diffBool, newComponentX, foreground);
			// 		updateComponentX(newComponentX, componentX, personListX, itemListX);
			// 		///
		}
		drawComponents(foreground, newFrameComponent);
		drawPersonPath(foreground, personList, posX, posY, click);
		drawTextStatus(foreground, personList, nonpersonList);
		drawComponents(f, newFrameComponent);
		drawPersonPath(f, personList, posX, posY, false);
		drawTextStatus(f, personList, nonpersonList);
		click = false;
		imshow("foreground", foreground);
		imshow("f", f);
		switch ( waitKey(10) ) {
			case 27: loop = false; break;
			case 'p': pause = !pause; break;
			case 'b':
				bghsv = !bghsv;
				cout << bghsv;
				break;
			case 'm': eachframe = !eachframe; break;
		}
		if (eachframe) {
			char c = waitKey(0);
			if (c == 'm') eachframe = !eachframe;
			else if (c == 27) loop = false;
		}
	}
	cout << "end video\n";
	//getchar();
}