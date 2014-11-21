#pragma once
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <list>
#include "Person.h"
#include "Item.h"

#define COMP_CHECKED 1
#define COMP_UNCHECKED 0

using namespace cv;
using namespace std;

class ComponentX {
public:

	Point cm;
	Point rect_tl;
	Point rect_br;
	int size=0;
	int checkStatus = COMP_UNCHECKED;
	vector<Point> path;
	Person a;
	vector<Person*> personListLink;
	vector<Item*> itemListLink;

	int id;
	ComponentX(Point cm, Point rect_tl, Point rect_br, int size, int id);
	void addPath(Point newPoint);
	~ComponentX();
};

