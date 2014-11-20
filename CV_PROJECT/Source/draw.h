#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include "Component.h"

#define RED Scalar (0,0,255)

void drawPersonPath(Mat &foreground,vector<Component> personList) {
#pragma omp parallel for
	for ( int i = 0; i < personList.size(); i++ ) {
		for ( int j = 1; j < personList[i].path.size(); j++ ) {
			line(foreground, personList[i].path[j], personList[i].path[j - 1], RED, 1, 8, 0);
		}
	}
}