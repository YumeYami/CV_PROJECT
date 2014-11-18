#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <list>
#include "Component.h"
#include "componentExtract.h"
#include "logic.h"
#include "bgSubtract.h"

#define INIT_SKIP_FRAME 20
#define WHITE Scalar(255,255,255)
#define RED Scalar (0,0,255)
using namespace cv;
using namespace std;

vector<Component> personList;
vector<Component> nonpersonList;
int thresholdCM = 100;
string imgNum = "1";
RNG rng(12345);

int main() {
	cout << "start...\n";
	VideoCapture cap("SampleVideo/IMG" + imgNum + ".mp4");
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
		imshow("background", f);
		bgSubtract(bg, f, diffBool, foreground);
		//bgSubtractRGB(bg, f, diffBool, foreground);
		vector<Component> newFrameComponent;
		findComponentContour(diffBool, newFrameComponent, foreground);
		//cout << "component num: " << newFrameComponent.size() << "\n";
		
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