#include "Component.h"

Component::Component() {
}

Component::Component(Point cm, Point rect_tl, Point rect_br, int size, int type, int id) {
	
}

Component::~Component() {
}

void Component::addPath(Point newPoint) {
	path.push_back(newPoint);
}
