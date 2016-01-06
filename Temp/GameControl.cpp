#include "GameControl.h"
#include <iostream>
#include "Model.h"

GameControl::GameControl(Model& newModel, View& newView) :model(newModel), view(newView){
	SDL_SetRelativeMouseMode(SDL_TRUE);
	keys = SDL_GetKeyboardState(nullptr);
	initEvents();
}

void GameControl::handleInput(){
	while (SDL_PollEvent(&currentEvent)){
		if (event_map[currentEvent.type] != nullptr){
			(this->*event_map[currentEvent.type])(&currentEvent);
		}
		
		/*
		if (currentEvent.type == SDL_MOUSEMOTION){
			view.camera.horizontal_angle -= currentEvent.motion.xrel*mouse_lookaround_speed;
			view.camera.vertical_angle -= currentEvent.motion.yrel*mouse_lookaround_speed;
		}
		*/
	}
	
	//must handle all events in the queue using SDL_PollEvent() to populate the keyboard state.
	if (keys[SDL_SCANCODE_R]){ model.setColor(1.0, 0.0, 0.0, 1.0); }
	if (keys[SDL_SCANCODE_G]){ model.setColor(0.0, 1.0, 0.0, 1.0); }
	if (keys[SDL_SCANCODE_B]){ model.setColor(0.0, 0.0, 1.0, 1.0); }
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
		model.add_event();
		break;
	case SDL_SCANCODE_UP:
		model.move(0);
		break;
	case SDL_SCANCODE_RIGHT:
		model.move(1);
		break;
	case SDL_SCANCODE_DOWN:
		model.move(2);
		break;
	case SDL_SCANCODE_LEFT:
		model.move(3);
		break;
	}
}

void GameControl::handleMouseMotion(SDL_Event* current_event){
	view.camera.horizontal_angle -= currentEvent.motion.xrel*mouse_lookaround_speed;
	view.camera.vertical_angle -= currentEvent.motion.yrel*mouse_lookaround_speed;
}