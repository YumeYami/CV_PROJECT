#ifndef _BG_DIFF
#define _BG_DIFF

#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <list>

#define DIFF_THRESHOLD 20

using namespace cv;
using namespace std;

inline void bgSubtract(Mat bg, Mat currFrame, Mat &diffBool, Mat &foreground) {
	Mat diff;
	absdiff(currFrame, bg, diff);
	diff.convertTo(diff, CV_8UC3);
	cvtColor(diff, diff, CV_BGR2GRAY);
	diffBool = diff > DIFF_THRESHOLD;
	cvtColor(diffBool, diff, CV_GRAY2BGR);
	diff &= currFrame;
	imshow("diff", diff);
	foreground = diff;
}

#endif // !_BG_DIFF