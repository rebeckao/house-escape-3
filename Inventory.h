#ifndef INVENTORY_H
#define INVENTORY_H
class Inventory;
#include "Item.h"
#include <vector>

class Inventory{
public:
	std::vector<Item*> inventory_items;
	size_t current_item_index = 0;	
	bool isEmpty();

	~Inventory();
};

#endif