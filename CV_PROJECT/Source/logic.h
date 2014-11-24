#pragma once
#include "Component.h"
#include "ComponentX.h"
int _threshold = 0;
int _thresholdSameThings = 5;

bool isOverlapped(Component a, Component b) {
	double cmDiff = pow(a.cm.x - b.cm.x, 2) + pow(a.cm.y - b.cm.y, 2);
	if ( cmDiff <= _threshold*_threshold ) {
		return true;
	}
	return false;
}

bool isSameThings(Component a, Component b) {
	double cmDiff = pow(a.cm.x - b.cm.x, 2) + pow(a.cm.y - b.cm.y, 2);
	if ( cmDiff <= _thresholdSameThings*_thresholdSameThings ) {
		return true;
	}
	return false;
}

void addToComponentList(int type, vector<Component> &componentList, Component &newComponent, int id) {
	newComponent.type = type;
	newComponent.id = id;
	componentList.push_back(newComponent);
	newComponent.status = CHECKED;
}

void updateComponent(vector<Component> &newComponent, vector<Component> &personList, vector<Component> &nonpersonList, int thresholdCM) {
	vector<Component> newPersonList;
	vector<Component> newNonpersonList;
	_threshold = thresholdCM;
	for ( unsigned int i = 0; i < newComponent.size(); i++ ) {
		if ( newComponent[i].status == CHECKED ) continue;
		int pID = 0;
		Component *person = nullptr;
		//check with person list
		for ( unsigned int j = 0; j < personList.size(); j++ ) {
			if ( !isOverlapped(newComponent[i], personList[j]) ) continue;
			//assume that new component is person
			person = &newComponent[i];
			bool splitObject = false;
			for ( unsigned int n = 0; n < personList[j].subComponents.size(); n++ ) {
				//component in personList is holding objects
				if ( isSameThings(personList[j].subComponents[n], *person) ) {
					//the object is same things with new component
					splitObject = true;
					personList[j].subComponents.erase(personList[j].subComponents.begin() + n);
					break;
				}
			}
			for ( unsigned int k = i + 1; k < newComponent.size(); k++ ) {
				if ( !isOverlapped(newComponent[k], personList[j]) )continue;
				//component in person list is split
				if ( personList[j].subComponents.size() != 0 ) {
					for ( unsigned int n = 0; n < personList[j].subComponents.size(); n++ ) {
						if ( isSameThings(personList[j].subComponents[n], newComponent[k]) ) {
							addToComponentList(NON_PERSON, newNonpersonList, newComponent[k], newComponent[k].id);
							personList[j].subComponents.erase(personList[j].subComponents.begin() + n);
							break;
						}
					}
				}
				else if ( newComponent[k].size > person->size ) {
					if ( splitObject )
						addToComponentList(NON_PERSON, newNonpersonList, *person, person->id);
					else
						addToComponentList(NON_PERSON, newNonpersonList, *person, personList[j].id);
					person = &newComponent[k];
				}
				else {
					addToComponentList(NON_PERSON, newNonpersonList, newComponent[k], personList[j].id);
				}
			}
			personList[j].addPath(person->cm);
			person->path = personList[j].path;
			person->subComponents = personList[j].subComponents;
			pID = personList[j].id;
			break;
		}
		if ( person != nullptr ) {
			if ( person->subComponents.size() != 0 ) {
				vector<Component>* subComponents = &person->subComponents;
				int pWidth = person->getWidth();
				int pHeight = person->getHeight();
				Point pCM = person->cm;
				for ( unsigned int i = 0; i < (*subComponents).size(); i++ ) {
					int sWidth = (*subComponents)[i].getWidth();
					int sHeight = (*subComponents)[i].getHeight();
					Point sCM = (*subComponents)[i].cm;
					bool farFromObject_x = 2 * abs(pCM.x - sCM.x) > pWidth + sWidth;
					bool farFromObject_y = 2 * abs(pCM.y - sCM.y) > pHeight + sHeight;
					if ( farFromObject_x || farFromObject_y ) {
						//person is far from object but component isn't split then the person is holding the object
						//and the object isn't belong to the person
						cout << "PersonID " << pID << " is Thief!!" << "\n";
					}
				}
			}
			for ( unsigned int j = 0; j < nonpersonList.size(); j++ ) {
				if ( isOverlapped(newComponent[i], nonpersonList[j]) ) {
					if ( nonpersonList[j].id != pID ) {
						person->addSubComponent(nonpersonList[j]);
					}
				}
			}
			addToComponentList(PERSON, newPersonList, *person, pID);
		}
		//new component is object
		else {
			for ( unsigned int j = 0; j < nonpersonList.size(); j++ ) {
				if ( isOverlapped(newComponent[i], nonpersonList[j]) ) {
					addToComponentList(NON_PERSON, newNonpersonList, newComponent[i], nonpersonList[j].id);
					break;
				}
			}
		}
		//new component is new person
		if ( newComponent[i].status != CHECKED ) {
			newComponent[i].addPath(newComponent[i].cm);
			addToComponentList(PERSON, newPersonList, newComponent[i], Person::personCounter++);
		}
	}
	personList = newPersonList;
	nonpersonList = newNonpersonList;
}

bool isSameComponentX(ComponentX &a, ComponentX &b) {
	double cmDiff = pow(a.cm.x - b.cm.x, 2) + pow(a.cm.y - b.cm.y, 2);
	if ( cmDiff <= _thresholdSameThings*_thresholdSameThings ) {
		return true;
	}
	return false;
}

void updateComponentX(vector<ComponentX> currComponents, vector<ComponentX> &prevComponents, vector<Person> &personList, vector<Item> &itemList, vector<Item> unknowItem) {
	vector<Component> newComp;
	///scan all curr component
	for ( unsigned int i = 0; i < currComponents.size(); i++ ) {
		if ( currComponents[i].checkStatus == CHECKED ) continue;
		///match with prev component
		bool isMatchWithPerson = false;
		for ( unsigned int j = 0; j < personList.size(); j++ ) {
			if ( personList[j].checkStatus == CHECKED ) continue;
			if ( isSameComponentX(currComponents[i], *personList[j].parent) ) {
				///found matchable prev component here
				isMatchWithPerson = true;
				///find other split curr component
				bool isFindSplitCurrComponent = false;
				for ( unsigned k = i + 1; k < currComponents.size(); k++ ) {
					if ( currComponents[k].checkStatus == CHECKED ) continue;
					if ( !isSameComponentX(currComponents[i], currComponents[k]) ) {
						///found split curr component here
						isFindSplitCurrComponent = true;
						if ( currComponents[i].size > currComponents[k].size ) {
							/// comp[i] is person
							personList[j].path.push_back(currComponents[i].cm);
							personList[j].parent = &currComponents[i];
						}
						else {
							/// comp[k] is person
						}
						break;
					}
				}
				break;
			}
		}
	}
}
