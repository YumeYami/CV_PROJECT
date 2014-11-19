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

void Component::addSubComponent(Component component){
	subComponents.push_back(component);
}

float Component::getWidth(){
	return rect_tl.x - rect_br.x;
}
float Component::getHeight(){
	return rect_tl.y - rect_br.y;
}
