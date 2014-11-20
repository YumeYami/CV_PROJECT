#include "NewComponent.h"


NewComponent::NewComponent() {
	this->cm = cm;
	this->rect_tl = rect_tl;
	this->rect_br = rect_br;
	this->size = size;
	this->id = id;
	addPath(cm);
}
void NewComponent::addPath(Point newPoint) {
	path.push_back(newPoint);
}

NewComponent::~NewComponent() {
}
