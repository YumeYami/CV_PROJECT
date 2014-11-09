#include "Component.h"

Component::Component() {
}

Component::Component(Point cm, Point rect_tl, Point rect_br, int size, vector<Point> path) {
	this->cm = cm;
	this->rect_tl = rect_tl;
	this->rect_br = rect_br;
	this->size = size;
	this->path = path;
}

Component::~Component() {
}

void Component::addPath(Point newPoint) {
	path.push_back(newPoint);
}
