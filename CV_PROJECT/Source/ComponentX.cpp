#include "ComponentX.h"

ComponentX::ComponentX(Point cm, Point rect_tl, Point rect_br, int size, int id) {
	this->cm = cm;
	this->rect_tl = rect_tl;
	this->rect_br = rect_br;
	this->size = size;
	this->id = id;
	addPath(cm);
}
void ComponentX::addPath(Point newPoint) {
	path.push_back(newPoint);
}

ComponentX::~ComponentX() {
}
