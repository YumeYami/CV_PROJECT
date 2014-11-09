#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <list>
#include "Component.h"
#include "ConnectedComponent.h"
#include "Logic.h"

#define THRESHOLD_CM 10
#define THRESHOLD_SIZE 50

using namespace cv;
using namespace std;

vector<Component> personList;
vector<Component> nonpersonList;

int main() {
	cout << "start...\n";
	Mat f;
	while ( true ) {
		vector<Component> newFrameComponent;
		findComponent(f, newFrameComponent);
		updateComponent(newFrameComponent, personList, nonpersonList, THRESHOLD_CM,THRESHOLD_SIZE);
	}
	waitKey(0);
	getchar();
}