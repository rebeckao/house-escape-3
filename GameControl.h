#ifndef CONTROL_H
#define CONTROL_H
#include "SDL.h"
#include "Model.h"
#include "View.h"

class GameControl{
protected:
	Model& model;
	View& view;

	const Uint8 *keys;
	SDL_Event currentEvent;

	virtual void initEvents() = 0;
	virtual void handleKeydown(SDL_Event* current_event) = 0;
	virtual void handleMouseMotion(SDL_Event* current_event) = 0;
public:
	//bool game_is_running = true;
	GameControl(Model&, View&);
	virtual void handleInput() = 0;
};

class BoardControl : public GameControl{
private:
	std::unordered_map<std::size_t, void(BoardControl::*)(SDL_Event*)> event_map;

	float camera_translation_speed = 0.15f;
	float mouse_lookaround_speed = 0.005f;

	virtual void initEvents() override;
	virtual void handleKeydown(SDL_Event* current_event) override;
	virtual void handleMouseMotion(SDL_Event* current_event) override;
public:
	BoardControl(Model&, View&);
	virtual void handleInput() override;
};

class InventoryControl : public GameControl{
private:
	std::unordered_map<std::size_t, void(InventoryControl ::*)(SDL_Event*)> event_map;

	virtual void initEvents() override;
	virtual void handleKeydown(SDL_Event* current_event) override;
	virtual void handleMouseMotion(SDL_Event* current_event) override;
public:
	InventoryControl(Model&, View&);
	virtual void handleInput() override;
};

#endif