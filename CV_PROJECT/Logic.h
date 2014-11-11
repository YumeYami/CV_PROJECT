#ifndef _LOGIC
#define _LOGIC

#include "personID.h"

bool isOverlapped(Component a, Component b, int thresholdCM){
	int cmDiff = pow(a.cm.x - b.cm.x, 2) + pow(a.cm.y - b.cm.y, 2);
	if (cmDiff <= thresholdCM){
		return true;
	}
	return false;
}

void updateComponent(vector<Component> &newComponent, vector<Component> &personList, vector<Component> &nonpersonList, int thresholdCM){
	vector<Component> newPersonList;
	vector<Component> newNonpersonList;

	for (int i = 0; i < newComponent.size(); i++){
		if (newComponent[i].id == CHECKED) continue;
		int pID = 0;
		for (int j = 0; j < personList.size(); j++){
			if (!isOverlapped(newComponent[i], personList[j], thresholdCM)) continue;
			Component *person = &newComponent[i];
			for (int k = i + 1; k < newComponent.size(); k++){
				if (!isOverlapped(newComponent[k], personList[j], thresholdCM))continue;
				if (newComponent[k].size > person->size){
					person->type = NON_PERSON;
					person->id = personList[j].id;
					newNonpersonList.push_back(*person);
					person->id = CHECKED;
					person = &newComponent[k];
				}
				else{
					newComponent[k].type = NON_PERSON;
					newComponent[k].id = personList[j].id;
					newNonpersonList.push_back(newComponent[k]);
					newComponent[k].id = CHECKED;
				}
			}
			person->type = PERSON;
			person->id = personList[j].id;
			newPersonList.push_back(*person);
			person->id = CHECKED;
			pID = personList[j].id;
			break;
		}
		if (pID!=0) {
			for (int j = 0; j < nonpersonList.size(); j++){
				if (isOverlapped(newComponent[i], nonpersonList[j], thresholdCM)){
					if (nonpersonList[j].id != pID) 
						cout << "PersonID "<< pID <<"is Thief!!";
					break;
				}
			}
		}
		else{
			for (int j = 0; j < nonpersonList.size(); j++){
				if (isOverlapped(newComponent[i], nonpersonList[j], thresholdCM)){
					newComponent[i].type = NON_PERSON;
					newComponent[i].id = nonpersonList[j].id;
					newNonpersonList.push_back(newComponent[i]);
					newComponent[i].id = CHECKED;
					break;
				}
			}
		}
		if (newComponent[i].id != CHECKED){
			newComponent[i].type = PERSON;
			newComponent[i].id = personID::serial++;
			newPersonList.push_back(newComponent[i]);
			newComponent[i].id = CHECKED;
		}
	}
	personList = newPersonList;
	nonpersonList = newNonpersonList;
	//add new path remain
}

#endif // !_LOGIC