#include "Component.h"

Component::Component() {
}

Component::Component(Point cm, Point rect_tl, Point rect_br, int size, int type, int id) {
	this->cm = cm;
	this->rect_br = rect_br;
	this->rect_tl = rect_tl;
	this->size = size;
	this->id = id;
	this->type = type;
}

Component::~Component() {
}

void Component::addPath(Point newPoint) {
	path.push_back(newPoint);
}

void Component::addSubComponent(Component component){
	subComponents.push_back(component);
}

int Component::getWidth(){
	return rect_tl.x - rect_br.x;
}
int Component::getHeight(){
	return rect_tl.y - rect_br.y;
}
