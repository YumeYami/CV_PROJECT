#ifndef _COMPONENT
#define _COMPONENT

#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <list>
using namespace cv;
using namespace std;

class Component {
public:
	Point cm;
	Point rect_tl;
	Point rect_br;
	int size;
	vector<Point> path;
	Component();
	Component(Point cm, Point rect_tl, Point rect_br, int size, vector<Point> path);
	void addPath(Point newPoint);
	~Component();
};
#endif // !_COMPONENT