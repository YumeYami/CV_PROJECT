#ifndef _CC
#define _CC

#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <queue>

#define DIRECTION 0					// 1 for 8-direction, 0 for 4-direction
#define THRESHOLD 150				// less than THRESHOLD is noise.

using namespace std;
using namespace cv;

inline Component fill_region(Mat, Point, int, Mat&);

inline void findComponent(Mat &frameDiff, vector<Component> &object) {
	// frameDiff is grayscale with 0 or 255
	Mat bool_traverse = Mat::zeros(frameDiff.rows, frameDiff.cols, CV_8U);
	int round = 1;
	for (int i = 0; i < frameDiff.rows; i++){
		for (int j = 0; j < frameDiff.cols; j++) {
			if (frameDiff.at<uchar>(i, j) > 128 && bool_traverse.at<uchar>(i, j) == 0) {
				//floodFill(frameDiff, Point(i, j), 199);
				Component component = fill_region(frameDiff, Point(i, j), round, bool_traverse);
				if (component.size > THRESHOLD) {
					object.push_back(component);
					if (round < 250) round++;
				}
			}
		}
	}
	//cout << "round: " << round << endl;
}

inline Component fill_region(Mat frameDiff, Point pos, int round, Mat &bool_traverse) {
	queue<Point> q;
	q.push(pos);
	int cnt = 0;
	int sumx = 0, sumy = 0, minx = 1000000000, miny = 1000000000, maxx = 0, maxy = 0;
	int kk = 10000;

	bool_traverse.at<uchar>(pos.x, pos.y) = round;
	while (!q.empty()) {
		Point p = q.front();	q.pop();

		sumx += p.x;				sumy += p.y;
		minx = min(minx, p.x);		miny = min(miny, p.y);
		maxx = max(maxx, p.x);		maxy = max(maxy, p.y);
		cnt++;


		if (p.y - 1 >= 0 && bool_traverse.at<uchar>(p.x, p.y - 1) == 0 && frameDiff.at<uchar>(p.x, p.y - 1) > 128)
		{
			bool_traverse.at<uchar>(p.x, p.y-1) = round;
			q.push(Point(p.x, p.y - 1));
		}
		if (p.y + 1 < frameDiff.cols && bool_traverse.at<uchar>(p.x, p.y + 1) == 0 && frameDiff.at<uchar>(p.x, p.y + 1) > 128)
		{
			bool_traverse.at<uchar>(p.x, p.y+1) = round;
			q.push(Point(p.x, p.y + 1));
		}
		if (p.x - 1 >= 0 && bool_traverse.at<uchar>(p.x - 1, p.y) == 0 && frameDiff.at<uchar>(p.x - 1, p.y) > 128)
		{
			bool_traverse.at<uchar>(p.x-1, p.y) = round;
			q.push(Point(p.x - 1, p.y));
		}
		if (p.x + 1 < frameDiff.rows && bool_traverse.at<uchar>(p.x + 1, p.y) == 0 && frameDiff.at<uchar>(p.x + 1, p.y) > 128)
		{
			bool_traverse.at<uchar>(p.x+1, p.y) = round;
			q.push(Point(p.x + 1, p.y));
		}

		if (DIRECTION) {
			if (p.x + 1 < frameDiff.rows && p.y + 1 < frameDiff.cols && bool_traverse.at<uchar>(p.x + 1, p.y + 1) == 0 &&
				frameDiff.at<uchar>(p.x + 1, p.y + 1) > 128)
			{
				q.push(Point(p.x + 1, p.y + 1));
			}
			if (p.x + 1 < frameDiff.rows && p.y - 1 >= 0 && bool_traverse.at<uchar>(p.x + 1, p.y - 1) == 0 &&
				frameDiff.at<uchar>(p.x + 1, p.y - 1) > 128)
			{
				q.push(Point(p.x + 1, p.y - 1));
			}
			if (p.x - 1 >= 0 && p.y + 1 < frameDiff.cols && bool_traverse.at<uchar>(p.x - 1, p.y + 1) == 0 &&
				frameDiff.at<uchar>(p.x - 1, p.y + 1) > 128)
			{
				q.push(Point(p.x - 1, p.y + 1));
			}
			if (p.x - 1 >= 0 && p.y - 1 >= 0 && bool_traverse.at<uchar>(p.x - 1, p.y - 1) == 0 &&
				frameDiff.at<uchar>(p.x - 1, p.y - 1) > 128)
			{
				q.push(Point(p.x - 1, p.y - 1));
			}
		}

	}
	
	if (cnt == 0) {
		printf("ERROR cnt = 0\n");
		cnt = 1;
	}
	Component component = Component(Point((int)(sumx / cnt), (int)(sumy / cnt)), Point(minx, miny), Point(maxx, maxy), cnt, 0, 0);
	return component;
}

#endif // !_CC