#ifndef CONTROL_H
#define CONTROL_H
#include "SDL.h"
#include "Model.h"
#include "View.h"

class GameControl{
	Model& model;
	View& view;

	const Uint8 *keys;
	SDL_Event currentEvent;
	std::unordered_map<std::size_t, void(GameControl::*)(SDL_Event*)> event_map;

	float camera_translation_speed = 0.15f;
	float mouse_lookaround_speed = 0.005f;

	void initEvents();
	void handleKeydown(SDL_Event* current_event);
	void handleMouseMotion(SDL_Event* current_event);
public:
	bool game_is_running = true;
	GameControl(Model&, View&);
	void handleInput();
};

#endif