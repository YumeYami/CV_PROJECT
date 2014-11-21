#pragma once
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include "Item.h"

using namespace cv;
using namespace std;

#define REAL_PERSON 1
#define UNKNOWN 0
#define UNCHECKED_PERSON 0
class ComponentX;
class Person {
public:
	static int personCounter;
	int id;
	int objectStatus = UNKNOWN;
	int checkStatus = UNCHECKED_PERSON;
	ComponentX* parent = NULL;
	Scalar color = Scalar(255, 255, 255);
	/// item which attached by this person
	vector<Item*> ownItemList;
	vector<Item*> nonOwnItemList;
	vector<Point> path;

	Person();
	~Person();
};

