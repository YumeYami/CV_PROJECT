#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include "Component.h"

#define RED Scalar (0,0,255)
#define FONT_SCALE 1
#define WHITE Scalar (255,255,255)

void drawComponents(Mat &foreground, vector<Component> components) {
	/// Draw polygonal contour + bonding rect + circles
	for ( unsigned int i = 0; i < components.size(); i++ ) {
		//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		Scalar color = Scalar(255, 255, 255);
		//drawContours(foreground, components, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(foreground, components[i].rect_tl, components[i].rect_br, components[i].color, 2, 8, 0);
		string str;
		if (components[i].type == PERSON) str = "Person: ";
		else if (components[i].type == NON_PERSON) str = "Item: ";
		else str = "UNKNOWN";

		if (str != "UNKNOWN") {
			putText(foreground, str + to_string(components[i].id), components[i].rect_tl, FONT_HERSHEY_PLAIN, FONT_SCALE, WHITE);
		}
		//circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);
	}
	//rectangle(foreground, Point(50, 50), Point(960-50, 540-50), Scalar(255, 0, 0), 2, 8, 0);
}

void drawPersonPath(Mat &foreground, vector<Component> &personList, int x, int y, bool click) {
	//#pragma omp parallel for
	for ( int i = 0; i < personList.size(); i++ ) {
		if ( click&&personList[i].isInComponent(x, y) ) {
			personList[i].showPath = !personList[i].showPath;

		}
		if ( personList[i].showPath ) {
			for ( int j = 1; j < personList[i].path.size(); j++ ) {
				line(foreground, personList[i].path[j], personList[i].path[j - 1], personList[i].color, 1, 8, 0);
			}
		}

	}
}

void drawTextStatus(Mat &foreground, vector<Component> personList, vector<Component>nonpersonList) {
	putText(foreground, "person: " + to_string(personList.size()) + " nonperson: " + to_string(nonpersonList.size()), Point(20, 20), FONT_HERSHEY_PLAIN, FONT_SCALE, WHITE);
	for ( unsigned int i = 0; i < personList.size(); i++ ) {
		if ( personList[i].isThief == THIEF ) {
			putText(foreground, "personID " + to_string(personList[i].id) + " is thief ", Point(20, 440), FONT_HERSHEY_PLAIN, FONT_SCALE, WHITE);
			string item = "item in thief: ";
			for ( unsigned j = 0; j < personList[i].subComponents.size(); j++ ) {
				item += to_string(personList[i].subComponents[j].id);
			}
			putText(foreground, item, Point(20, 490), FONT_HERSHEY_PLAIN, FONT_SCALE, WHITE);
		}
		else if (personList[i].subComponents.size() > 0) {
			string item = "item in person " + to_string(personList[i].id) + ": ";
			for (unsigned j = 0; j < personList[i].subComponents.size(); j++) {
				item += to_string(personList[i].subComponents[j].id);
			}
			putText(foreground, item, Point(20, 490), FONT_HERSHEY_PLAIN, FONT_SCALE, WHITE);
		}
	}

	// draw Instruction
	putText(foreground, "Press p to pause", Point(650, 480), FONT_HERSHEY_PLAIN, FONT_SCALE, WHITE);
	putText(foreground, "Click person to show the path", Point(650, 510), FONT_HERSHEY_PLAIN, FONT_SCALE, WHITE);
}