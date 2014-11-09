#ifndef _LOGIC
#define _LOGIC

#endif // !_LOGIC

void mergeComponent(Component &nearestComponent, Component &newComponent){
	nearestComponent.cm = newComponent.cm;
	nearestComponent.rect_br = newComponent.rect_br;
	nearestComponent.rect_tl = newComponent.rect_tl;
	nearestComponent.addPath(newComponent.path[0]);
	nearestComponent.size = newComponent.size;
}
void updateComponent(vector<Component> &newComponent, vector<Component> &personList, vector<Component> &nonpersonList, int thresholdCM,int thresholdSize){
	
	vector<Component> personTemp;
	vector<Component> nonpersonTemp;

	for (int j = 0; j < newComponent.size();j++){
		Component nearestComponent;
		int type,index;
		if (nonpersonList.size() > 0){
			nearestComponent = nonpersonList[0];
			type = 0;
			index = 0;
		}
		else if (personList.size()>0){
			nearestComponent = personList[0];
			type = 1;
			index = 0;
		}
		else break;
		
		int min = pow(nearestComponent.cm.x - newComponent[j].cm.x, 2) + pow(nearestComponent.cm.y - newComponent[j].cm.y, 2);
		for (int i = 0; i < nonpersonList.size(); i++){
			int cmDiff = pow(nonpersonList[i].cm.x - newComponent[j].cm.x, 2) + pow(nonpersonList[i].cm.y - newComponent[j].cm.y, 2);
			int sizeDiff = abs(nonpersonList[i].size - newComponent[j].size);
			if (cmDiff < min && sizeDiff <= thresholdSize){
				min = cmDiff;
				nearestComponent = nonpersonList[i];
				type = 0;
				index = i;
			}
		}
		for (int i = 0; i < personList.size(); i++){
			int cmDiff = pow(personList[i].cm.x - newComponent[j].cm.x, 2) + pow(personList[i].cm.y - newComponent[j].cm.y, 2);
			int sizeDiff = abs(personList[i].size - newComponent[j].size);
			if (cmDiff < min && sizeDiff <= thresholdSize){
				min = cmDiff;
				nearestComponent = personList[i];
				type = 1;
				index = i;
			}
		}
		int sizeDiff = abs(nearestComponent.size - newComponent[j].size);
		if (min <= thresholdCM && sizeDiff<= thresholdSize){
			if (type == 0){
				mergeComponent(nearestComponent, newComponent[j]);
				
			}
		}
	}
}

