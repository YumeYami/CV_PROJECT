#pragma once
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>


using namespace cv;

#define INDIV_ITEM 0
#define NON_ASSIGN 0
class ComponentX;
class Person;
class Item {
public:
	Person *parent = NULL;
	static int itemCounter;
	int itemID = NON_ASSIGN;//item
	int ownerID = INDIV_ITEM;//person
	int holderID = NON_ASSIGN;//person
	int size = NON_ASSIGN;
	Scalar color = Scalar(255, 255, 255);
	Item();
	~Item();
};