#include "ControlManager.h"

ControlManager::ControlManager(Model& input_model, View& input_view) :
board_controller(BoardControl(input_model, input_view)), inventory_controller(InventoryControl(input_model, input_view)), model(input_model), view(input_view){
	active_controller = &board_controller;
	initEvents();
}

ControlManager::~ControlManager(){

}

void ControlManager::handleInput(){
	handleEvents();
	active_controller->handleInput();
}

void ControlManager::handleEvents(){
	for (size_t i = 0; i < model.control_event_queue.size(); i++){
		callEvent(model.control_event_queue[i]);
	}
	clearEvents();
}

void ControlManager::callEvent(ControlEvent*& control_event){
	(this->*(event_map[control_event->event_ID]))(control_event);
}

void ControlManager::clearEvents(){
	for (size_t i = 0; i < model.control_event_queue.size(); i++){
		delete model.control_event_queue[i];
	}
	model.control_event_queue.clear();
}

void ControlManager::initEvents(){
	void(ControlManager::* member_function_pointer)(ControlEvent*);

	member_function_pointer = &ControlManager::selectController;
	event_map[ControlEventID::SELECT_CONTROLLER] = member_function_pointer;
}

void ControlManager::selectController(ControlEvent* select_controller_event){
	SelectControllerEvent* current_event = dynamic_cast<SelectControllerEvent*>(select_controller_event);
	switch (current_event->controller_id){
	case ControlerID::BOARD_CONTROLLER: 
		active_controller = &board_controller;
		break;
	case ControlerID::INVENTORY_CONTROLLER:
		active_controller = &inventory_controller;
		break;
	}
}