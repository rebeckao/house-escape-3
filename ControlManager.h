#ifndef CONTROLMANAGER_H
#define CONTROLMANAGER_H
#include "GameControl.h"
#include "View.h"
#include "Model.h"
#include "ControlEvent.h"


class ControlManager{
private:
	InventoryControl inventory_controller;
	BoardControl board_controller;	
	std::unordered_map<ControlEventID, void(ControlManager::*)(ControlEvent*)> event_map;
	Model& model;
	View& view;

	void handleEvents();
	void initEvents();
	void selectController(ControlEvent* select_controller_event);
	void callEvent(ControlEvent*& control_event);
	void clearEvents();
public:
	GameControl* active_controller;

	ControlManager(Model& model, View& view);
	~ControlManager();
	void handleInput();
};
#endif