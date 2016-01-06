#ifndef CONTROLEVENT_H
#define CONTROLEVENT_H

enum class ControlEventID{
	SELECT_CONTROLLER,
};

enum class ControlerID{
	BOARD_CONTROLLER,
	INVENTORY_CONTROLLER
};


class ControlEvent{
public:
	ControlEventID event_ID;
	ControlEvent(ControlEventID input_event_ID);
	virtual ~ControlEvent();
};

class SelectControllerEvent :public ControlEvent{
public:
	ControlEventID derived_event_ID = ControlEventID::SELECT_CONTROLLER;
	ControlerID controller_id;
	SelectControllerEvent(ControlerID controller_id);
};

#endif