//#include <tclDecls.h>
#include "Model.h"
#include <algorithm>

Model::Model(){
	initEvents();
	active_player_token = new PlayerToken();
	CarrotItem* carrot = new CarrotItem();
	active_player_token->inventory->inventory_items.push_back(carrot);

	the_monster = new SpiderMonster(100,1,*this);

	gameBoard = new GameBoard();
	model_event_queue.push_back(new InsertTokenModelEvent(active_player_token, 6, 3));
	model_event_queue.push_back(new ExploreTileModelEvent(gameBoard->tile_matrix[6][3], 6, 3));

	model_event_queue.push_back(new InsertTokenModelEvent(the_monster, 0, 0));
	model_event_queue.push_back(new ExploreTileModelEvent(gameBoard->tile_matrix[0][0], 0, 0));
	view_event_queue.push_back(new InitGameBoardViewEvent(gameBoard->tile_matrix.size(), gameBoard->tile_matrix[0].size()));
}

Model::~Model(){
	delete gameBoard;
}

void timeStepModel(Model& model, double timeStep){
	model.HandleEvents();
	model.the_monster->action();
}

//Event functions
/////////////////////////////////////////////////////////////////////////////////////
void Model::initEvents() {
	void(Model::* member_function_pointer)(ModelEvent*);

	member_function_pointer = &Model::findKey;
	event_map[MODEL_EVENT_FIND_KEY] = member_function_pointer;

	member_function_pointer = &Model::moveToken;
	event_map[MODEL_EVENT_MOVE_TOKEN] = member_function_pointer;

	member_function_pointer = &Model::inputDirection;
	event_map[MODEL_INPUT_DIRECTION] = member_function_pointer;

	member_function_pointer = &Model::useCurrentItem;
	event_map[MODEL_EVENT_USE_CURRENT_ITEM] = member_function_pointer;

	member_function_pointer = &Model::insertToken;
	event_map[MODEL_EVENT_CREATE_TOKEN] = member_function_pointer;

	member_function_pointer = &Model::exploreTile;
	event_map[MODEL_EVENT_EXPLORE_TILE] = member_function_pointer;

	member_function_pointer = &Model::inventorySelection;
	event_map[MODEL_INPUT_INVENTORY_DIRECTION] = member_function_pointer;

	member_function_pointer = &Model::toggleInventory;
	event_map[MODEL_INPUT_INVENTORY_TOGGLE] = member_function_pointer;
}


void Model::addModelEvent(ModelEvent* current_event) {
	model_event_queue.push_back(current_event);
}

void Model::HandleEvents() {
	for (size_t i = 0; i < model_event_queue.size(); ++i) {
		callEvent(model_event_queue[i]);
	}
	clearEvents();
}

void Model::callEvent(ModelEvent*& model_event) {
	(this->*(event_map[model_event->event_ID]))(model_event);
}

void Model::clearEvents() {
	for (size_t i = 0; i < model_event_queue.size(); ++i) {
		delete model_event_queue[i];
	}
	model_event_queue.clear();
}

void Model::moveToken(ModelEvent* move_token_event) {
	MoveTokenModelEvent* current_event = dynamic_cast<MoveTokenModelEvent*>(move_token_event);
	if (gameBoard->move_token(current_event->token, current_event->direction)){
		view_event_queue.push_back(new MoveTokenDirectionViewEvent(current_event->direction,current_event->token));
		std::vector<size_t> position = gameBoard->token_locations[current_event->token];
		if (!gameBoard->tile_matrix[position[0]][position[1]]->explored){
			ExploreTileModelEvent explore_tile_event(gameBoard->tile_matrix[position[0]][position[1]],position[0],position[1]);
			exploreTile(&explore_tile_event);
		}				
	}
}

void Model::inputDirection(ModelEvent* input_direction_event) {
	InputDirectionModelEvent* current_event = dynamic_cast<InputDirectionModelEvent*>(input_direction_event);
	//todo: switch-cases of different gamestates
	MoveTokenModelEvent move_direction_event(active_player_token, current_event->direction);
	moveToken(&move_direction_event);
}

void Model::insertToken(ModelEvent* create_token_event){
	InsertTokenModelEvent* current_event = dynamic_cast<InsertTokenModelEvent*>(create_token_event);
	gameBoard->insert_token(current_event->token, current_event->i, current_event->j);
	if (current_event->token == active_player_token){
		view_event_queue.push_back(new CreateTokenViewEvent(current_event->i, current_event->j, current_event->token, std::string("ninjaclown_player.bmp")));
	}else if (current_event->token == the_monster){
		view_event_queue.push_back(new CreateTokenViewEvent(current_event->i, current_event->j, current_event->token, std::string("spider_token.bmp")));
	}else{
		view_event_queue.push_back(new CreateTokenViewEvent(current_event->i, current_event->j, current_event->token, std::string("samus.bmp")));
	}	
}

void Model::exploreTile(ModelEvent* explore_tile_event){
	ExploreTileModelEvent* current_event = dynamic_cast<ExploreTileModelEvent*>(explore_tile_event);
	Tile* current_tile = current_event->tile;
	if (current_event->tile->explored == false){
		current_event->tile->explored = true;
		view_event_queue.push_back(new TileExploredViewEvent(current_event->i,current_event->j,current_event->tile->exits));
		if (current_event->i == 2 && current_event->j == 2){
			FindKeyModelEvent* find_key_event = &FindKeyModelEvent(active_player_token);
			findKey(find_key_event);
		}
	}
}

void Model::findKey(ModelEvent* find_key_event) {
	FindKeyModelEvent* current_event = dynamic_cast<FindKeyModelEvent*>(find_key_event);

	KeyItem* key_item = new KeyItem();
	active_player_token->inventory->inventory_items.push_back(key_item);
	std::cout << "You found the key! Now, to return from whence you came..." << std::endl;
}

void Model::inventorySelection(ModelEvent* inventory_input_direction_event){
	InputInventoryDirectionModelEvent* current_event = dynamic_cast<InputInventoryDirectionModelEvent*>(inventory_input_direction_event);	
	Inventory & current_inventory = *active_player_token->inventory;
	if (current_inventory.inventory_items.size() == 0){
		std::cout << "Your inventory is empty!" << std::endl;
	}else{
		++active_player_token->inventory->current_item_index;
		current_inventory.current_item_index %= current_inventory.inventory_items.size();
		std::cout << "Your selected item is:" << current_inventory.inventory_items[current_inventory.current_item_index]->name << std::endl;
	}	
}

void Model::toggleInventory(ModelEvent* toggle_inventory_input_event){
	InputToggleInventoryModelEvent* current_event = dynamic_cast<InputToggleInventoryModelEvent*>(toggle_inventory_input_event);
	inventory_is_open = !inventory_is_open;
	if (inventory_is_open){
		control_event_queue.push_back(new SelectControllerEvent(ControlerID::INVENTORY_CONTROLLER));
		std::cout << "You opened the inventory! Your current items are:" << std::endl;
		Inventory& current_inventory = *active_player_token->inventory;
		for (size_t i = 0; i < active_player_token->inventory->inventory_items.size(); i++){
			std::cout << current_inventory.inventory_items[i]->name << " ";
		}
		std::cout << std::endl;
		if (current_inventory.isEmpty()){
			std::cout << "Your inventory is empty!" << std::endl;
		}else{
			std::cout << "Your selected item is:" << current_inventory.inventory_items[current_inventory.current_item_index]->name << std::endl;
		}

	}else{
		control_event_queue.push_back(new SelectControllerEvent(ControlerID::BOARD_CONTROLLER));
		std::cout << "You closed the inventory!" << std::endl;
	}
}

void Model::useCurrentItem(ModelEvent* use_current_item_event){
	UseCurrentItemModelEvent* current_event = dynamic_cast<UseCurrentItemModelEvent*>(use_current_item_event);
	if (!active_player_token->inventory->isEmpty()){
		active_player_token->inventory->inventory_items[active_player_token->inventory->current_item_index]->useEffect(*this);
	}
}