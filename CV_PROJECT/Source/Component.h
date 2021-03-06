#pragma once
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <list>
#include "Person.h"
#include "Item.h"

#define NOISE -1

#define UNKNOWN 0
#define PERSON -4
#define NON_PERSON -5
#define GROUP -6

#define CHECKED -3
#define UNCHECKED -2

#define THIEF true
#define NOT_THIEF false

using namespace cv;
using namespace std;

class Component {
public:
	Point cm;
	Point rect_tl;
	Point rect_br;
	int size;
	vector<Point> path;
	vector<Component> subComponents;
	Point oldPosition = Point(0, 0), oldoldPosition = Point(0,0);
	int type;
	int id;
	int status = UNCHECKED;
	int mergeStatus = UNCHECKED;
	bool showPath;
	bool isThief = NOT_THIEF;
	Scalar color;

	Component();
	Component(Point cm, Point rect_tl, Point rect_br, int size, int type, int id);
	void addPath(Point newPoint);
	void addSubComponent(Component component);
	int getWidth();
	int getHeight();
	bool isInComponent(int x, int y);
	void Component::setColor();
	~Component();
};
