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
#define PERSON 1
#define NON_PERSON 2

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
	vector<Component> subComponents;
	int type;
	int id;
	bool showPath;
	Component();
	Component(Point cm, Point rect_tl, Point rect_br, int size, int type, int id);
	void addPath(Point newPoint);
	void addSubComponent(Component component);
	int getWidth();
	int getHeight();
	bool isInComponent(int x, int y);
	~Component();
};
