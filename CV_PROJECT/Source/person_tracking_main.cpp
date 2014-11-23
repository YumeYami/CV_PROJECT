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

int thresholdCM = 100;
string imgNum = "6";
RNG rng(12345);

int main() {
	cout << "start...\n";
	//VideoCapture cap("SampleVideo/IMG" + imgNum + ".mp4");
	VideoCapture cap("video/video0" + imgNum + ".mp4");
	//VideoCapture cap(0);
	if ( !cap.isOpened() ) {
		cout << "video error\n";
		return -1;
	}
	Mat f, diffBool, bg, foreground;
	///skip empty frame
	do { cap >> f; } while ( f.empty() );
	///skip initial frame
	//for ( int i = 0; i < INIT_SKIP_FRAME; i++ ) cap >> f;
	cap >> bg;
	bool loop = true, showPath = true;
	while ( loop ) {
		cap >> f;
		if ( f.empty() ) break;
		imshow("background", f);
		bgSubtract(bg, f, diffBool, foreground);
		//bgSubtractRGB(bg, f, diffBool, foreground);
		///old version
		vector<Component> newFrameComponent;
		findComponentContour(diffBool, newFrameComponent, foreground);
		updateComponent(newFrameComponent, personList, nonpersonList, thresholdCM);
// 		///new version
// 		vector<ComponentX> newComponentX;
// 		findComponentXContour(diffBool, newComponentX, foreground);
// 		updateComponentX(newComponentX, componentX, personListX, itemListX);
// 		///

		cout << "person: " << personList.size() << " nonperson: " << nonpersonList.size() << "\n";
		if ( !personList.empty() ) {
			cout << "personID: ";
			for (unsigned int i = 0; i < personList.size(); i++ ) {
				cout << personList[i].id << " ";
			}
			cout << "\n";
		}
		drawComponents(foreground, newFrameComponent);
		if ( showPath ) {
			drawPersonPath(foreground, personList);
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