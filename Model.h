#ifndef MODEL_H
#define MODEL_H
class Model;
#include <vector>
#include <list>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include "GameBoard.h"
#include "ModelEvent.h"
#include "ViewEvent.h"
#include "ControlEvent.h"

class Model{
private:
public:
	std::unordered_map<std::size_t, void(Model::*)(ModelEvent*)> event_map;
	std::vector<ModelEvent*> model_event_queue;
	std::vector<ViewEvent*> view_event_queue;
	std::vector<ControlEvent*> control_event_queue;
	GameBoard* gameBoard;
	bool game_is_running = true;
	
	PlayerToken* active_player_token;
	bool inventory_is_open = false;

	SpiderMonster* the_monster;

	Model();
	~Model();
	void addModelEvent(ModelEvent* current_event);
	void HandleEvents();
	void callEvent(ModelEvent*& model_event);
	void clearEvents();

	//Model event functions
	void initEvents();
	void moveToken(ModelEvent* move_token_event);
	void insertToken(ModelEvent* create_token_event);
	void exploreTile(ModelEvent* explore_tile);	
	void findKey(ModelEvent* find_key_event);
	void useCurrentItem(ModelEvent* use_current_item_event);

	void toggleInventory(ModelEvent* toggle_inventory_input_event);
	void inventorySelection(ModelEvent* inventory_input_direction_event);
	void inputDirection(ModelEvent* input_direction_event);
};

void timeStepModel(Model& model, double timeStep);

#endif