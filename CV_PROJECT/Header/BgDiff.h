#ifndef _BG_DIFF
#define _BG_DIFF

#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <list>

#define DIFF_THRESHOLD 20
#define DIFF_RGB_THRESHOLD 60

using namespace cv;
using namespace std;

inline void bgSubtract(Mat bg, Mat currFrame, Mat &diffBool, Mat &foreground) {
	Mat diff;
	absdiff(currFrame, bg, diff);
	diff.convertTo(diff, CV_8UC3);

	cvtColor(diff, diff, CV_BGR2GRAY);
	diffBool = diff > DIFF_THRESHOLD;
	cvtColor(diffBool, diff, CV_GRAY2BGR);
	//diff &= currFrame;
	imshow("diff", diff);
	foreground = diff;
}
inline void bgSubtractRGB(Mat bg, Mat currFrame, Mat &diffBool, Mat &foreground) {
	Mat diff;
	absdiff(currFrame, bg, diff);
	diff.convertTo(diff, CV_8UC3);
	Mat diffRGB[3];
	split(diff, diffRGB);
	diffBool = diffRGB[0];
	//#pragma omp parallel for
	for ( int i = 1; i < 3; i++ ) {
		diffBool += diffRGB[i];
	}
	diffBool = diffBool > DIFF_RGB_THRESHOLD;
	//cvtColor(diff, diff, CV_BGR2GRAY);
	//diffBool = diff > DIFF_THRESHOLD;
	cvtColor(diffBool, diff, CV_GRAY2BGR);
	//diff &= currFrame;
	imshow("diff", diff);
	foreground = diff;
}

#endif // !_BG_DIFF