#ifndef _COMPONENT
#define _COMPONENT

#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <list>

#define PERSON 0
#define NON_PERSON 1
#define CHECKED -1
using namespace cv;
using namespace std;

class Component {
public:
	Point cm;
	Point rect_tl;
	Point rect_br;
	int size;
	vector<Point> path;
	int type;
	int id;
	Component();
	Component(Point cm, Point rect_tl, Point rect_br, int size, int type, int id);
	void addPath(Point newPoint);
	~Component();
};
#endif // !_COMPONENT