#ifndef TOKEN_H
#define TOKEN_H
class Token;
class PlayerToken;
class MonsterToken;
class SpiderMonster;
#include <string>
#include "Inventory.h"

class Token {
public:
	Token();
	virtual ~Token();
	std::string name;
	int max_health;
	int current_health;
	int strength;
	int agility;
	int armor;
	int luck;
	Inventory* inventory;
	//std::vector<Item> inventory;

	std::string getName() const;
};

class PlayerToken : public Token{

};

class MonsterToken : public Token{
protected:
	int action_threshold = 1000;
	int action_progression = 0;
	int monster_speed = 2;
	Model& model;
public:
	MonsterToken(Model& model);
	virtual void action() = 0;
};

class SpiderMonster : public MonsterToken{
public:
	SpiderMonster(const int & action_threshold, const int & monster_speed, Model& model);
	void moveClockwise();
	virtual void action() override;
	void move();
	void attack();
};

#endif