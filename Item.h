#ifndef ITEM_H
#define ITEM_H
class Item;
#include <string>
#include "Model.h"
//#include "ModelEvent.h"

class Item {
public:
	Item();
	virtual ~Item();
	std::string name;
	std::string description;
	double weight = 1.0;

	std::string getDescription();
	unsigned int value = 0;	
	virtual void useEffect(Model &model) = 0;
};

class KeyItem : public Item {
public:
	KeyItem();
	virtual void useEffect(Model &model) override;
};

class PotionItem : public Item {
public:
	PotionItem();
	virtual void useEffect(Model &model) override;
};

class CarrotItem : public Item{
public:
	CarrotItem();
	virtual void useEffect(Model &model) override;
};

#endif