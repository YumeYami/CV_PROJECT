#pragma once
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "morphology.h"
#include <iostream>
#include <list>

#define DIFF_THRESHOLD 20
#define DIFF_H_THRESHOLD 50
#define DIFF_S_THRESHOLD 100
#define DIFF_V_THRESHOLD 100
#define OPENING_SIZE 2
#define CLOSING_SIZE 3
#define OPENING 0
#define CLOSING 1

using namespace cv;
using namespace std;

inline void bgSubtract(Mat bg, Mat currFrame, Mat &diffBool, Mat &foreground) {
	Mat diff;
	absdiff(currFrame, bg, diff);
	diff.convertTo(diff, CV_8UC3);

	cvtColor(diff, diff, CV_BGR2GRAY);
	diffBool = diff > DIFF_THRESHOLD;
	imshow("diffBool", diffBool);

	Morphology_Operations(diffBool, diffBool, OPENING, 0, OPENING_SIZE);
	Morphology_Operations(diffBool, diffBool, CLOSING, 0, CLOSING_SIZE);
	cvtColor(diffBool, diff, CV_GRAY2BGR);
	//diff &= currFrame;
	//imshow("diff", diff);
	foreground = diff;
}
inline void bgSubtractHSV(Mat bg, Mat currFrame, Mat &diffBool, Mat &foreground) {
	Mat diff,bgHSV,currHSV;
	diff.convertTo(diff, CV_8UC3);
	cvtColor(currFrame, currHSV, CV_BGR2HSV);
	cvtColor(bg, bgHSV, CV_BGR2HSV);
	absdiff(currHSV, bgHSV, diff);

	Mat diffHSV[3];
	split(diff, diffHSV);
	Mat tmp1, tmp2;
	///not same color
	Mat g = diffHSV[0] > 90;
	g /= 255;
	g *= 180;
	absdiff(diffHSV[0], g, diffHSV[0]);
	diffBool = diffHSV[0] > DIFF_H_THRESHOLD;

	///same color
	tmp1 = diffHSV[0] < DIFF_H_THRESHOLD;
	tmp1 &= diffHSV[1] > DIFF_S_THRESHOLD;
	tmp1 &= diffHSV[2] > DIFF_V_THRESHOLD;
	diffBool |= tmp1;
	
	imshow("diffBool", diffBool);

	Morphology_Operations(diffBool, diffBool, OPENING, 0, OPENING_SIZE);
	Morphology_Operations(diffBool, diffBool, CLOSING, 0, CLOSING_SIZE);
	//diff &= currFrame;
	//imshow("diff", diff);
	cvtColor(diffBool, diff, CV_GRAY2BGR);
	foreground = diff;
}