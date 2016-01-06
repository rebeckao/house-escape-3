#include "GameControl.h"
#include <iostream>

GameControl::GameControl(Model& newModel, View& newView) :model(newModel), view(newView){
	SDL_SetRelativeMouseMode(SDL_TRUE);
	keys = SDL_GetKeyboardState(nullptr);
	//initEvents();
}


BoardControl::BoardControl(Model& new_model, View& new_view) :GameControl(new_model,new_view){
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	//keys = SDL_GetKeyboardState(nullptr);
	initEvents();
}

void BoardControl::handleInput(){
	while (SDL_PollEvent(&currentEvent)){
		if (event_map[currentEvent.type] != nullptr){
			(this->*event_map[currentEvent.type])(&currentEvent);
		}
	}

	//State functions. Must handle all events in the queue using SDL_PollEvent() to populate the keyboard state.
	if (keys[SDL_SCANCODE_W]){ view.camera.position += view.camera.direction*camera_translation_speed; }
	if (keys[SDL_SCANCODE_A]){ view.camera.position -= view.camera.right*camera_translation_speed; }
	if (keys[SDL_SCANCODE_S]){ view.camera.position -= view.camera.direction*camera_translation_speed; }
	if (keys[SDL_SCANCODE_D]){ view.camera.position += view.camera.right*camera_translation_speed; }
	if (keys[SDL_SCANCODE_SPACE]){ view.camera.position += view.camera.up*camera_translation_speed; }

	if (keys[SDL_SCANCODE_ESCAPE]){ model.game_is_running = false; }
}

void BoardControl::initEvents(){
	void(BoardControl::* member_function_pointer)(SDL_Event*);

	member_function_pointer = &BoardControl::handleKeydown;
	event_map[SDL_KEYDOWN] = member_function_pointer;

	member_function_pointer = &BoardControl::handleMouseMotion;
	event_map[SDL_MOUSEMOTION] = member_function_pointer;
}

void BoardControl::handleKeydown(SDL_Event* input_event){
	switch (currentEvent.key.keysym.scancode){
	case SDL_SCANCODE_RETURN:
		break;
	case SDL_SCANCODE_UP:
		model.addModelEvent(new InputDirectionModelEvent(GAMEBOARD_NORTH));
		break;
	case SDL_SCANCODE_RIGHT:
		model.addModelEvent(new InputDirectionModelEvent(GAMEBOARD_EAST));
		break;
	case SDL_SCANCODE_DOWN:
		model.addModelEvent(new InputDirectionModelEvent(GAMEBOARD_SOUTH));
		break;
	case SDL_SCANCODE_LEFT:
		model.addModelEvent(new InputDirectionModelEvent(GAMEBOARD_WEST));
		break;
	case SDL_SCANCODE_P:
		break;
		//model.addModelEvent(new CreateTokenModelEvent(model.gameBoard.token_locations[model.gameBoard.active_player_token][0], model.gameBoard.token_locations[(model.gameBoard.active_player_token)][1]));
	case SDL_SCANCODE_I:
		model.addModelEvent(new InputToggleInventoryModelEvent());
		break;
		
	}
}

void BoardControl::handleMouseMotion(SDL_Event* current_event){
	view.camera.horizontal_angle -= currentEvent.motion.xrel*mouse_lookaround_speed;
	view.camera.vertical_angle -= currentEvent.motion.yrel*mouse_lookaround_speed;
}


InventoryControl::InventoryControl(Model& new_model, View& new_view) :GameControl(new_model, new_view){
	initEvents();
}

void InventoryControl::handleInput(){
	while (SDL_PollEvent(&currentEvent)){
		if (event_map[currentEvent.type] != nullptr){
			(this->*event_map[currentEvent.type])(&currentEvent);
		}
	}

	//State functions. Must handle all events in the queue using SDL_PollEvent() to populate the keyboard state.
	if (keys[SDL_SCANCODE_ESCAPE]){ model.game_is_running = false; }
}

void InventoryControl::initEvents(){
	void(InventoryControl ::* member_function_pointer)(SDL_Event*);

	member_function_pointer = &InventoryControl::handleKeydown;
	event_map[SDL_KEYDOWN] = member_function_pointer;

	member_function_pointer = &InventoryControl::handleMouseMotion;
	event_map[SDL_MOUSEMOTION] = member_function_pointer;
}

void InventoryControl::handleKeydown(SDL_Event* input_event){
	switch (currentEvent.key.keysym.scancode){
	case SDL_SCANCODE_RETURN: //Todo: use item
		break;
	case SDL_SCANCODE_RIGHT:
		model.addModelEvent(new InputInventoryDirectionModelEvent(GAMEBOARD_EAST));
		break;
	case SDL_SCANCODE_LEFT:
		model.addModelEvent(new InputInventoryDirectionModelEvent(GAMEBOARD_WEST));
		break;
	case SDL_SCANCODE_E:
		model.addModelEvent(new UseCurrentItemModelEvent());
		break;
	case SDL_SCANCODE_I:
		model.addModelEvent(new InputToggleInventoryModelEvent());
		break;
	}
}

void InventoryControl::handleMouseMotion(SDL_Event* current_event){
}


/*
void GameControl::handleInput(){
	while (SDL_PollEvent(&currentEvent)){
		if (event_map[currentEvent.type] != nullptr){
			(this->*event_map[currentEvent.type])(&currentEvent);
		}
	}
	
	//must handle all events in the queue using SDL_PollEvent() to populate the keyboard state.
	if (keys[SDL_SCANCODE_W]){ view.camera.position += view.camera.direction*camera_translation_speed; }
	if (keys[SDL_SCANCODE_A]){ view.camera.position -= view.camera.right*camera_translation_speed; }
	if (keys[SDL_SCANCODE_S]){ view.camera.position -= view.camera.direction*camera_translation_speed; }
	if (keys[SDL_SCANCODE_D]){ view.camera.position += view.camera.right*camera_translation_speed; }
	if (keys[SDL_SCANCODE_SPACE]){ view.camera.position += view.camera.up*camera_translation_speed; }

	if (keys[SDL_SCANCODE_ESCAPE]){ game_is_running = false;}
}

void GameControl::initEvents(){
	void(GameControl::* member_function_pointer)(SDL_Event*);

	member_function_pointer = &GameControl::handleKeydown;
	event_map[SDL_KEYDOWN] = member_function_pointer;

	member_function_pointer = &GameControl::handleMouseMotion;
	event_map[SDL_MOUSEMOTION] = member_function_pointer;
}

void GameControl::handleKeydown(SDL_Event* input_event){
	switch (currentEvent.key.keysym.scancode){
	case SDL_SCANCODE_RETURN:
		break;
	case SDL_SCANCODE_UP:
		model.addModelEvent(new InputDirectionModelEvent(GAMEBOARD_NORTH));
		break;
	case SDL_SCANCODE_RIGHT:
		model.addModelEvent(new InputDirectionModelEvent(GAMEBOARD_EAST));
		break;
	case SDL_SCANCODE_DOWN:
		model.addModelEvent(new InputDirectionModelEvent(GAMEBOARD_SOUTH));
		break;
	case SDL_SCANCODE_LEFT:
		model.addModelEvent(new InputDirectionModelEvent(GAMEBOARD_WEST));
		break;
	case SDL_SCANCODE_P:
		break;
		//model.addModelEvent(new CreateTokenModelEvent(model.gameBoard.token_locations[model.gameBoard.active_player_token][0], model.gameBoard.token_locations[(model.gameBoard.active_player_token)][1]));
	}
}

void GameControl::handleMouseMotion(SDL_Event* current_event){
	view.camera.horizontal_angle -= currentEvent.motion.xrel*mouse_lookaround_speed;
	view.camera.vertical_angle -= currentEvent.motion.yrel*mouse_lookaround_speed;
}
*/