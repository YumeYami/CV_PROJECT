#pragma once
#include "Component.h"
#include "ComponentX.h"

// define MERGE_THRESHOLD
#include "extractor.h"

#define THRESHOLD_PERSON 300

int _threshold = 0;
int _thresholdSameThings = 5;
int FRAME_WIDTH = 0;
int FRAME_HEIGHT = 0;
void setVideoSize(int width, int height) {
	FRAME_WIDTH = width;
	FRAME_HEIGHT = height;
}

bool isOverlapped(Component a, Component b) {
	//double cmDiff = pow(a.cm.x - b.cm.x, 2) + pow(a.cm.y - b.cm.y, 2);
	//if ( cmDiff <= _threshold*_threshold ) {
	//	return true;
	//}
	//return false;
	if (abs(a.cm.x - b.cm.x) < (a.getWidth() + b.getWidth()) / 2 && abs(a.cm.y - b.cm.y) < (a.getHeight() + b.getHeight()) / 2 )
		return true;
	else
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
	newComponent.setColor();
	if (type == PERSON) {
		newComponent.oldPosition = newComponent.cm;
	}
	componentList.push_back(newComponent);
	newComponent.status = CHECKED;
}

void updateComponent(vector<Component> &newComponent, vector<Component> &personList, vector<Component> &nonpersonList, vector<Component> &groupList, int thresholdCM) {
	vector<Component> newPersonList;
	vector<Component> newNonpersonList;
	vector<Component> newGroupList;
	_threshold = thresholdCM;
	int flag = 0;

	for ( unsigned int i = 0; i < newComponent.size(); i++ ) {
		if ( newComponent[i].status == CHECKED ) continue;
		int pID = 0;
		
		Component *person = nullptr;

		// check split person - check component groupList addToComponentList(PERSON, ... ) 
		for (int j = 0; j < groupList.size(); j++) {
			if (isOverlapped(newComponent[i], groupList[j])) {
				//flag = 1;
				int flag2 = 0;
				for (int k = i + 1; k < newComponent.size(); k++) {
					if (isOverlapped(newComponent[k], groupList[j])) {
						flag2 = 1;
						//groupList[j].subComponents[1]
						Vector<int> v1, v2, v3, v4;	// 1, 2 before merge, 3, 4 after merge
						//v1.push_back(groupList[j].cm.x - groupList[j].subComponents[0].cm.x);
						//v1.push_back(groupList[j].cm.y - groupList[j].subComponents[0].cm.y);
						//v2.push_back(groupList[j].cm.x - groupList[j].subComponents[1].cm.x);
						//v2.push_back(groupList[j].cm.y - groupList[j].subComponents[1].cm.y);
						v1.push_back(groupList[j].subComponents[0].cm.x - groupList[j].subComponents[0].oldoldPosition.x);
						v1.push_back(groupList[j].subComponents[0].cm.y - groupList[j].subComponents[0].oldoldPosition.y);
						v2.push_back(groupList[j].subComponents[1].cm.x - groupList[j].subComponents[1].oldoldPosition.x);
						v2.push_back(groupList[j].subComponents[1].cm.y - groupList[j].subComponents[1].oldoldPosition.y);

						//v3.push_back(newComponent[i].cm.x - groupList[j].cm.x);
						//v3.push_back(newComponent[i].cm.y - groupList[j].cm.y);
						//v4.push_back(newComponent[k].cm.x - groupList[j].cm.x);
						//v4.push_back(newComponent[k].cm.y - groupList[j].cm.y);
						v3.push_back(newComponent[i].cm.x - groupList[j].subComponents[0].cm.x);
						v3.push_back(newComponent[i].cm.y - groupList[j].subComponents[0].cm.y);
						v4.push_back(newComponent[k].cm.x - groupList[j].subComponents[1].cm.x);
						v4.push_back(newComponent[k].cm.y - groupList[j].subComponents[1].cm.y);

						//cout << groupList[j].subComponents[0].cm << " " << groupList[j].subComponents[0].id << " " << groupList[j].subComponents[1].cm << " " << groupList[j].subComponents[1].id << "\n";
						//printf("%d %d %d %d\n", newComponent[i].cm.x, newComponent[i].cm.y, newComponent[k].cm.x, newComponent[k].cm.y);
						//printf("dot [%d %d %d] [%d %d] %d\n", v1[0], v1[1], groupList[j].subComponents[0].id, v3[0], v3[1], dot(v1, v3));
						if (dot(v1, v3) > 0) {		// > 0 means เดินสวนกัน => subComponent[0] equals newComponent[i]
							printf("1 %d %d\n", groupList[j].subComponents[0].id, groupList[j].subComponents[1].id);
							newComponent[i].path = groupList[j].subComponents[0].path;
							newComponent[k].path = groupList[j].subComponents[1].path;
							addToComponentList(PERSON, newPersonList, newComponent[i], groupList[j].subComponents[0].id);
							addToComponentList(PERSON, newPersonList, newComponent[k], groupList[j].subComponents[1].id);
						}
						else {
							printf("2 %d %d\n", groupList[j].subComponents[0].id, groupList[j].subComponents[1].id);
							newComponent[i].path = groupList[j].subComponents[1].path;
							newComponent[k].path = groupList[j].subComponents[0].path;
							addToComponentList(PERSON, newPersonList, newComponent[i], groupList[j].subComponents[1].id);
							addToComponentList(PERSON, newPersonList, newComponent[k], groupList[j].subComponents[0].id);
						}
						groupList.erase(groupList.begin() + j);
						break;
					}
				}
				if (flag2 == 0) {
					newComponent[i].subComponents.push_back(groupList[j].subComponents[0]);
					newComponent[i].subComponents.push_back(groupList[j].subComponents[1]);
					addToComponentList(GROUP, newGroupList, newComponent[i], groupList[j].id);
				}
			}
		}

		// check merge person - เอา component ไปไล่ดูใน personList ถ้า overlap > 1 => กลายเป็น group ดังนั้น addToComponentList(GROUP, ... ) ใน groupList
		int cnt = 0;
		for (int j = 0; j < personList.size(); j++) {
			if (isOverlapped(newComponent[i], personList[j])) {
				cnt++;
			}
		}
		if (cnt > 1) {
			for (int j = 0; j < personList.size(); j++) {
				if (isOverlapped(newComponent[i], personList[j])) {
					//personList[j].oldPosition = personList[j].cm;
					newComponent[i].addSubComponent(personList[j]);
				}
			}
			flag = 3;
			//printf("flag 3: %d %d %d\n", newComponent[i].subComponents[0].id, newComponent[i].subComponents[1].id, newComponent[i].subComponents.size());
			addToComponentList(GROUP, newGroupList, newComponent[i], newGroupList.size() + 1);
			continue;
		}

		//check with person list
		for ( unsigned int j = 0; j < personList.size(); j++ ) {
			if ( !isOverlapped(newComponent[i], personList[j]) ) continue;
			//assume that new component is person
			person = &newComponent[i];
			for ( unsigned int k = i + 1; k < newComponent.size(); k++ ) {
				if ( !isOverlapped(newComponent[k], personList[j]) )continue;
				//component in person list is split
				
				if (newComponent[k].size > person->size) {
					bool newObject = true;
					if (personList[j].subComponents.size() != 0) {
						for (unsigned int n = 0; n < personList[j].subComponents.size(); n++) {
							if (isSameThings(personList[j].subComponents[n], *person)) {
								newObject = false;
								addToComponentList(NON_PERSON, newNonpersonList, *person, personList[j].subComponents[n].id);
								personList[j].subComponents.erase(personList[j].subComponents.begin() + n);
								break;
							}
						}
					}
					if (newObject){
						addToComponentList(NON_PERSON, newNonpersonList, *person, personList[j].id);
					}
					person = &newComponent[k];
				}
				else {
					bool newObject = true;
					if (personList[j].subComponents.size() != 0) {
						for (unsigned int n = 0; n < personList[j].subComponents.size(); n++) {
							if (isSameThings(personList[j].subComponents[n], newComponent[k])) {
								newObject = false;
								addToComponentList(NON_PERSON, newNonpersonList, newComponent[k], personList[j].subComponents[n].id);
								personList[j].subComponents.erase(personList[j].subComponents.begin() + n);
								break;
							}
						}
					}
					if (newObject){
						addToComponentList(NON_PERSON, newNonpersonList, newComponent[k], personList[j].id);
					}
				}
			}
			personList[j].addPath(person->cm);
			person->oldoldPosition = personList[j].cm;
			person->path = personList[j].path;
			person->showPath = personList[j].showPath;
			person->subComponents = personList[j].subComponents;
			pID = personList[j].id;
			break;
		}
		if ( person != nullptr ) {
			if ( person->subComponents.size() != 0 ) {		// เอาของไปด้วยหรือเปล่า
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
					//if ( farFromObject_x || farFromObject_y ) {
					if (!isMergeable(*person, (*subComponents)[i])) {
						//person is far from object but component isn't split then the person is holding the object
						//and the object isn't belong to the person
						person->isThief = THIEF;
						//cout << (*person).cm << " " << (*person).getWidth() << " " << (*person).getHeight() << "\n";
						//cout << (*person).rect_tl << " " << (*person).rect_br << "\n";
						//cout << (*subComponents)[i].cm << " " << (*subComponents)[i].getWidth() << " " << (*subComponents)[i].getHeight() << "\n";
						
						cout << "PersonID " << pID << " is Thief!! " << "\n";
					}
				}
			}
			for ( unsigned int j = 0; j < nonpersonList.size(); j++ ) {		// เดินผ่านของหรือเปล่า
				if ( isOverlapped(*person, nonpersonList[j]) ) {
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
			if (abs(newComponent[i].rect_br.x-FRAME_WIDTH) < FRAME_WIDTH - 50 && abs(newComponent[i].rect_br.y-FRAME_HEIGHT) < FRAME_HEIGHT - 50 
				&& newComponent[i].size < THRESHOLD_PERSON) {
				
			}
			else {
				addToComponentList(PERSON, newPersonList, newComponent[i], Person::personCounter++);
			}
		}
	}
	personList = newPersonList;
	nonpersonList = newNonpersonList;
	groupList = newGroupList;
}
//
//bool isSameComponentX(ComponentX &a, ComponentX &b) {
//	double cmDiff = pow(a.cm.x - b.cm.x, 2) + pow(a.cm.y - b.cm.y, 2);
//	if ( cmDiff <= _thresholdSameThings*_thresholdSameThings ) {
//		return true;
//	}
//	return false;
//}
//
//void updateComponentX(vector<ComponentX> currComponents, vector<ComponentX> &prevComponents, vector<Person> &personList, vector<Item> &itemList, vector<Item> unknowItem) {
//	vector<Component> newComp;
//	///scan all curr component
//	for ( unsigned int i = 0; i < currComponents.size(); i++ ) {
//		if ( currComponents[i].checkStatus == CHECKED ) continue;
//		///match with prev component
//		bool isMatchWithPerson = false;
//		for ( unsigned int j = 0; j < personList.size(); j++ ) {
//			if ( personList[j].checkStatus == CHECKED ) continue;
//			if ( isSameComponentX(currComponents[i], *personList[j].parent) ) {
//				///found matchable prev component here
//				isMatchWithPerson = true;
//				///find other split curr component
//				bool isFindSplitCurrComponent = false;
//				for ( unsigned k = i + 1; k < currComponents.size(); k++ ) {
//					if ( currComponents[k].checkStatus == CHECKED ) continue;
//					if ( !isSameComponentX(currComponents[i], currComponents[k]) ) {
//						///found split curr component here
//						isFindSplitCurrComponent = true;
//						if ( currComponents[i].size > currComponents[k].size ) {
//							/// comp[i] is person
//							personList[j].path.push_back(currComponents[i].cm);
//							personList[j].parent = &currComponents[i];
//						}
//						else {
//							/// comp[k] is person
//						}
//						break;
//					}
//				}
//				break;
//			}
//		}
//	}
//}
