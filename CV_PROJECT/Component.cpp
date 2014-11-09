#include "Component.h"


Component::Component() {
}

Component::Component(Point cm, Point rect_tl, Point rect_br, int size) {
	this->cm = cm;
	this->rect_tl = rect_tl;
	this->rect_br = rect_br;
	this->size = size;
}

Component::~Component() {
}
