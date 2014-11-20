#pragma once

#define INDIV_ITEM 0
#define NON_ASSIGN 0

class Item {
public:
	static int itemCounter;
	int itemID = NON_ASSIGN;//item
	int ownerID = INDIV_ITEM;//person
	int holderID = NON_ASSIGN;//person
	int size = NON_ASSIGN;
	Item();
	~Item();
};