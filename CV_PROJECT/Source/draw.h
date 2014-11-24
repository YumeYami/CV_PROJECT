#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include "Component.h"

#define RED Scalar (0,0,255)

void drawComponents(Mat &foreground, vector<Component> components) {
	/// Draw polygonal contour + bonding rect + circles
	for ( unsigned int i = 0; i < components.size(); i++ ) {
		//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		Scalar color = Scalar(255, 255, 255);
			//drawContours(foreground, components, i, color, 1, 8, vector<Vec4i>(), 0, Point());
			rectangle(foreground, components[i].rect_tl, components[i].rect_br, color, 2, 8, 0);
		//circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);
	}
}

void drawPersonPath(Mat &foreground,vector<Component> &personList, int x, int y,bool click) {
//#pragma omp parallel for
	for ( int i = 0; i < personList.size(); i++ ) {
		if (click&&personList[i].isInComponent(x, y)){
			personList[i].showPath = !personList[i].showPath;
			
		}
		if (personList[i].showPath){
			for (int j = 1; j < personList[i].path.size(); j++) {
				line(foreground, personList[i].path[j], personList[i].path[j - 1], RED, 1, 8, 0);
			}
		}
	
	}
}