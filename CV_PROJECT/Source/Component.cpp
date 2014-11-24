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
	this->showPath = false;
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
	return rect_br.x - rect_tl.x;
}
int Component::getHeight(){
	return rect_br.y - rect_tl.y;
}
bool Component::isInComponent(int x, int y){
	if (x <= rect_br.x&&x >= rect_tl.x&&y <= rect_br.y&&y >= rect_tl.y){
		return true;
	}
	return false;
}