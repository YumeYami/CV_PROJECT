#include "Header/Component.h"

Component::Component() {
}

Component::Component(Point cm, Point rect_tl, Point rect_br, int size, int type, int id) {
	this->cm = cm;
	this->rect_tl = rect_tl;
	this->rect_br = rect_br;
	this->size = size;
	this->type = type;
	this->id = id;
	addPath(cm);	
}

Component::~Component() {
}

void Component::addPath(Point newPoint) {
	path.push_back(newPoint);
}
