#ifndef MODELEVENT_H
#define MODELEVENT_H
class ModelEvent;
#include "Token.h"
#include "Tile.h"
#include "GameBoard.h"

enum Directions {
	GAMEBOARD_NORTH,
	GAMEBOARD_EAST,
	GAMEBOARD_SOUTH,
	GAMEBOARD_WEST
};

enum ModelEventID {
	MODEL_EVENT_EXPLORE_TILE,
	MODEL_EVENT_FIND_KEY,
	MODEL_EVENT_MOVE_TOKEN,
	MODEL_EVENT_CREATE_TOKEN,
	MODEL_EVENT_USE_CURRENT_ITEM,
	MODEL_INPUT_DIRECTION,
	MODEL_INPUT_INVENTORY_DIRECTION,
	MODEL_INPUT_INVENTORY_TOGGLE,
};


class ModelEvent{
public:
	size_t event_ID;
	ModelEvent(size_t input_event_ID);
	ModelEvent();
	virtual ~ModelEvent();
};

class MoveTokenModelEvent : public ModelEvent {
public:
	size_t derived_event_ID = MODEL_EVENT_MOVE_TOKEN;
	Token* token;
	Directions direction;
	MoveTokenModelEvent(Token* token, Directions direction);
};

class InputDirectionModelEvent : public ModelEvent {
public:
	InputDirectionModelEvent(Directions direction);
	size_t derived_event_ID = MODEL_INPUT_DIRECTION;
	Directions direction;
};

class InsertTokenModelEvent : public ModelEvent {
public:
	InsertTokenModelEvent(Token* token, size_t i, size_t j);
	size_t derived_event_ID = MODEL_EVENT_CREATE_TOKEN;
	Token* token;
	size_t i;
	size_t j;
};

class ExploreTileModelEvent : public ModelEvent {
public:
	Tile* tile;
	size_t i, j, derived_event_ID = MODEL_EVENT_EXPLORE_TILE;
	ExploreTileModelEvent(Tile* tile, size_t i, size_t j);	
};

class InputInventoryDirectionModelEvent : public ModelEvent {
public:	
	size_t derived_event_ID = MODEL_INPUT_INVENTORY_DIRECTION;
	Directions direction;
	InputInventoryDirectionModelEvent(Directions direction);
};

class InputToggleInventoryModelEvent : public ModelEvent {
public:
	size_t derived_event_ID = MODEL_INPUT_INVENTORY_TOGGLE;
	InputToggleInventoryModelEvent();
};

class UseCurrentItemModelEvent : public ModelEvent {
public:
	UseCurrentItemModelEvent();
	size_t derived_event_ID = MODEL_EVENT_USE_CURRENT_ITEM;
};

class FindKeyModelEvent : public ModelEvent {
public:
	FindKeyModelEvent(Token* player_token);
	size_t derived_event_ID = MODEL_EVENT_FIND_KEY;
	Token* player_token;
};

#endif