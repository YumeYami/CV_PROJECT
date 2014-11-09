#pragma once
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
	Component();
	Component(Point cm, Point rect_tl, Point rect_br, int size);
	~Component();
};

