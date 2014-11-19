#pragma once
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <list>
#include "Person.h"
#include "Item.h"

#define PERSON 0
#define NON_PERSON 1
#define CHECKED -1

using namespace cv;
using namespace std;

class NewComponent {
public:

	Point cm;
	Point rect_tl;
	Point rect_br;
	int size;
	vector<Point> path;
	vector<Person*> personList;
	vector<Item*> itemList;
	int id;
	NewComponent();
	NewComponent(Point cm, Point rect_tl, Point rect_br, int size, int id);
	void addPath(Point newPoint);
	NewComponent();
	~NewComponent();
};

