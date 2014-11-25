#pragma once
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <queue>
#include <algorithm>

#define DIRECTION 0					// 1 for 8-direction, 0 for 4-direction
#define AREA_THRESHOLD 20				// less than THRESHOLD is noise.
#define MERGE_THRESHOLD 30

using namespace std;
using namespace cv;

inline Component fill_region(Mat, Point, int, Mat&);

inline bool isMergeable(Component a, Component b) {
	if ( abs(a.cm.x - b.cm.x) < (a.getWidth() + b.getWidth()) / 2 + MERGE_THRESHOLD && abs(a.cm.y - b.cm.y) < (a.getHeight() + b.getHeight()) / 2 + MERGE_THRESHOLD )
		return true;
	else
		return false;
}
inline Component mergeComponent(Component a, Component b) {
	Point tl = Point(min(a.rect_tl.x, b.rect_tl.x), min(a.rect_tl.y, b.rect_tl.y));
	Point br = Point(max(a.rect_br.x, b.rect_br.x), max(a.rect_br.y, b.rect_br.y));
	Point cm = Point((tl.x + br.x) / 2, (tl.y + br.y) / 2);
	return Component(cm, tl, br, a.size + b.size, UNKNOWN, UNKNOWN);
}
inline void findComponentContour(Mat &diffBool, vector<Component> &object, Mat &foreground) {
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	///Detect edges using Threshold
	//threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
	/// Find contours
	findContours(diffBool, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());
	vector<Component> components;
	///threshold area of contours
	for ( unsigned int i = 0; i < contours.size(); i++ ) {
		double area = contourArea(contours[i]);
		if ( area < AREA_THRESHOLD ) {

		}
		else {
			boundRect[i] = boundingRect(Mat(contours[i]));
			Component newCom = Component(Point((boundRect[i].br().x + boundRect[i].tl().x) / 2, (boundRect[i].br().y + boundRect[i].tl().y) / 2), boundRect[i].tl(), boundRect[i].br(), area, UNKNOWN, UNKNOWN);
			components.push_back(newCom);
		}
	}
	///merge small component
	for ( unsigned i = 0; i < components.size(); i++ ) {
		if ( components[i].mergeStatus == CHECKED ) continue;
		//components[i].mergeStatus = CHECKED;
		bool flag = true;
		while ( flag ) {
			flag = false;
			for ( unsigned int j = 0; j < components.size(); j++ ) {
				if ( components[j].mergeStatus == CHECKED || i == j ) continue;
				if ( isMergeable(components[i], components[j]) ) {
					components[i] = mergeComponent(components[i], components[j]);
					components[j].mergeStatus = CHECKED;
					flag = true;
					//cout << "merge!!" << i << " " << j << "\n";
				}
			}
		}
		object.push_back(components[i]);
	}
	
}


inline void findComponent(Mat &frameDiff, vector<Component> &object) {
	// frameDiff is grayscale with 0 or 255
	Mat bool_traverse = Mat::zeros(frameDiff.rows, frameDiff.cols, CV_8U);
	int round = 1;
	for ( int i = 0; i < frameDiff.rows; i++ ) {
		for ( int j = 0; j < frameDiff.cols; j++ ) {
			if ( frameDiff.at<uchar>(i, j) > 128 && bool_traverse.at<uchar>(i, j) == 0 ) {
				//floodFill(frameDiff, Point(i, j), 199);
				Component component = fill_region(frameDiff, Point(i, j), round, bool_traverse);
				if ( component.size > AREA_THRESHOLD ) {
					object.push_back(component);
					if ( round < 250 ) round++;
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
	while ( !q.empty() ) {
		Point p = q.front();	q.pop();

		sumx += p.x;				sumy += p.y;
		minx = min(minx, p.x);		miny = min(miny, p.y);
		maxx = max(maxx, p.x);		maxy = max(maxy, p.y);
		cnt++;


		if ( p.y - 1 >= 0 && bool_traverse.at<uchar>(p.x, p.y - 1) == 0 && frameDiff.at<uchar>(p.x, p.y - 1) > 128 ) {
			bool_traverse.at<uchar>(p.x, p.y - 1) = round;
			q.push(Point(p.x, p.y - 1));
		}
		if ( p.y + 1 < frameDiff.cols && bool_traverse.at<uchar>(p.x, p.y + 1) == 0 && frameDiff.at<uchar>(p.x, p.y + 1) > 128 ) {
			bool_traverse.at<uchar>(p.x, p.y + 1) = round;
			q.push(Point(p.x, p.y + 1));
		}
		if ( p.x - 1 >= 0 && bool_traverse.at<uchar>(p.x - 1, p.y) == 0 && frameDiff.at<uchar>(p.x - 1, p.y) > 128 ) {
			bool_traverse.at<uchar>(p.x - 1, p.y) = round;
			q.push(Point(p.x - 1, p.y));
		}
		if ( p.x + 1 < frameDiff.rows && bool_traverse.at<uchar>(p.x + 1, p.y) == 0 && frameDiff.at<uchar>(p.x + 1, p.y) > 128 ) {
			bool_traverse.at<uchar>(p.x + 1, p.y) = round;
			q.push(Point(p.x + 1, p.y));
		}

		if ( DIRECTION ) {
			if ( p.x + 1 < frameDiff.rows && p.y + 1 < frameDiff.cols && bool_traverse.at<uchar>(p.x + 1, p.y + 1) == 0 &&
				frameDiff.at<uchar>(p.x + 1, p.y + 1) > 128 ) {
				q.push(Point(p.x + 1, p.y + 1));
			}
			if ( p.x + 1 < frameDiff.rows && p.y - 1 >= 0 && bool_traverse.at<uchar>(p.x + 1, p.y - 1) == 0 &&
				frameDiff.at<uchar>(p.x + 1, p.y - 1) > 128 ) {
				q.push(Point(p.x + 1, p.y - 1));
			}
			if ( p.x - 1 >= 0 && p.y + 1 < frameDiff.cols && bool_traverse.at<uchar>(p.x - 1, p.y + 1) == 0 &&
				frameDiff.at<uchar>(p.x - 1, p.y + 1) > 128 ) {
				q.push(Point(p.x - 1, p.y + 1));
			}
			if ( p.x - 1 >= 0 && p.y - 1 >= 0 && bool_traverse.at<uchar>(p.x - 1, p.y - 1) == 0 &&
				frameDiff.at<uchar>(p.x - 1, p.y - 1) > 128 ) {
				q.push(Point(p.x - 1, p.y - 1));
			}
		}

	}

	if ( cnt == 0 ) {
		printf("ERROR cnt = 0\n");
		cnt = 1;
	}
	Component component = Component(Point((int)(sumx / cnt), (int)(sumy / cnt)), Point(minx, miny), Point(maxx, maxy), cnt, 0, 0);
	return component;
}
