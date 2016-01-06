#ifndef VIEWEVENT_H
#define VIEWEVENT_H
class ViewEvent;
#include "Token.h"
#include <vector>
#include "ModelEvent.h"

enum ViewEventID{
	VIEW_EVENT_EXPLORE_TILE,
	VIEW_EVENT_MEGA_EXPLOSION,
	VIEW_EVENT_CREATE_TOKEN,
	VIEW_EVENT_MOVE_TOKEN,
	VIEW_EVENT_MOVE_TOKEN_DIRECTION,
	VIEW_EVENT_INIT_GAME_BOARD
};

class ViewEvent{
public:
	size_t event_ID;
	ViewEvent(size_t input_event_ID);
	virtual ~ViewEvent();
};

class TileExploredViewEvent :public ViewEvent{
public:
	size_t i, j, derived_event_ID = VIEW_EVENT_EXPLORE_TILE;
	std::vector < bool > room_exits;
	TileExploredViewEvent(size_t i, size_t j, std::vector < bool > room_exits);
};

class MegaExplosionViewEvent : public ViewEvent{
public:
	size_t i, j, derived_event_ID = VIEW_EVENT_MEGA_EXPLOSION;
	MegaExplosionViewEvent(size_t i, size_t j);
};

class CreateTokenViewEvent : public ViewEvent{
public:
	size_t i, j, derived_event_ID = VIEW_EVENT_CREATE_TOKEN;
	std::string token_file_name;
	Token* token;
	CreateTokenViewEvent(size_t i, size_t j, Token* token, std::string token_file_name);
};

class MoveTokenViewEvent : public ViewEvent{
public:
	size_t i, j, derived_event_ID = VIEW_EVENT_MOVE_TOKEN;
	Token* token;
	MoveTokenViewEvent(size_t i, size_t j, Token* token);
};

class MoveTokenDirectionViewEvent : public ViewEvent{
public:
	size_t derived_event_ID = VIEW_EVENT_MOVE_TOKEN_DIRECTION;
	Directions direction;
	Token* token;
	MoveTokenDirectionViewEvent(Directions direction, Token* token);
};

class InitGameBoardViewEvent : public ViewEvent{
public:
	size_t width,height,derived_event_ID = VIEW_EVENT_INIT_GAME_BOARD;
	InitGameBoardViewEvent(size_t width, size_t height);
};

#endif