#include "Inventory.h"

Inventory::~Inventory(){
	for (size_t i = 0; i < inventory_items.size(); i++){
		delete inventory_items[i];
	}
	inventory_items.clear();
}

bool Inventory::isEmpty(){
	return inventory_items.size() == 0;
}