#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <list>
#include "Component.h"
#include "ConnectedComponent.h"
#include "BgDiff.h"

#define INIT_SKIP_FRAME 20

using namespace cv;
using namespace std;

vector<Component> personList;
vector<Component> nonpersonList;

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
	while ( true ) {
		cap >> f;
		if ( f.empty() ) break;
		imshow("background", bg);
		bgSubtract(bg, f, diffBool);
		vector<Component> newFrameComponent;
		findComponent(diffBool, newFrameComponent);
		waitKey(10);
	}
	cout << "end video\n";
	waitKey(0);
	getchar();
}