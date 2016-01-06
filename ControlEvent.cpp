#include "ControlEvent.h"

ControlEvent::~ControlEvent(){}
ControlEvent::ControlEvent(ControlEventID input_event_ID) :event_ID(input_event_ID){}

SelectControllerEvent::SelectControllerEvent(ControlerID controller_id) :
	ControlEvent(ControlEventID::SELECT_CONTROLLER), controller_id(controller_id){}
