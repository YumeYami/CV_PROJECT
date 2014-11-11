#ifndef _LOGIC
#define _LOGIC

#include "personID.h"
int _threshold = 0;

bool isOverlapped(Component a, Component b){
	int cmDiff = pow(a.cm.x - b.cm.x, 2) + pow(a.cm.y - b.cm.y, 2);
	if (cmDiff <= _threshold*_threshold){
		return true;
	}
	return false;
}

void addToComponentList(int type,vector<Component> &componentList, Component &newComponent, int id){
	newComponent.type = type;
	newComponent.id = id;
	componentList.push_back(newComponent);
	newComponent.id = CHECKED;
}

void updateComponent(vector<Component> &newComponent, vector<Component> &personList, vector<Component> &nonpersonList, int thresholdCM){
	vector<Component> newPersonList;
	vector<Component> newNonpersonList;
	_threshold = thresholdCM;

	for (int i = 0; i < newComponent.size(); i++){
		if (newComponent[i].id == CHECKED) continue;
		int pID = 0;
		for (int j = 0; j < personList.size(); j++){
			if (!isOverlapped(newComponent[i], personList[j])) continue;
			Component *person = &newComponent[i];
			for (int k = i + 1; k < newComponent.size(); k++){
				if (!isOverlapped(newComponent[k], personList[j]))continue;
				if (newComponent[k].size > person->size){
					addToComponentList(NON_PERSON, newNonpersonList, *person, personList[j].id);
					person = &newComponent[k];
				}
				else{
					addToComponentList(NON_PERSON, newNonpersonList, newComponent[k], personList[j].id);
				}
			}
			personList[j].addPath(person->cm);
			person->path = personList[j].path;
			addToComponentList(PERSON, newPersonList, *person, personList[j].id);
			pID = personList[j].id;
			break;
		}
		if (pID!=0) { 
			for (int j = 0; j < nonpersonList.size(); j++){
				if (isOverlapped(newComponent[i], nonpersonList[j])){
					if (nonpersonList[j].id != pID) 
						cout << "PersonID "<< pID <<"is Thief!!";
					break;
				}
			}
		}
		else{
			for (int j = 0; j < nonpersonList.size(); j++){
				if (isOverlapped(newComponent[i], nonpersonList[j])){
					addToComponentList(NON_PERSON, newNonpersonList, newComponent[i], nonpersonList[j].id);
					break;
				}
			}
		}
		if (newComponent[i].id != CHECKED){ //newComponent[i] is new person
			newComponent[i].addPath(newComponent[i].cm);
			addToComponentList(PERSON, newPersonList, newComponent[i], personID::serial++);
		}
	}
	personList = newPersonList;
	nonpersonList = newNonpersonList;
}

#endif // !_LOGIC