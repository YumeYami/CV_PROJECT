#pragma once
#include <iostream>
#include <opencv2/core/core.hpp>
#include "Item.h"

using namespace cv;
using namespace std;

class Person {
public:
	static int personCounter;
	int id;
	Vec3i color;
	vector<Item> ownItemList;
	vector<Item> nonOwnItemList;
	vector<Point> path;

	Person();
	~Person();
};

