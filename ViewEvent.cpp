#include "ViewEvent.h"
ViewEvent::~ViewEvent(){}
ViewEvent::ViewEvent(size_t input_event_ID) :event_ID(input_event_ID){}

TileExploredViewEvent::TileExploredViewEvent(size_t i, size_t j, std::vector <bool> room_exits) :
	ViewEvent(VIEW_EVENT_EXPLORE_TILE), i(i), j(j), room_exits(room_exits){}
MegaExplosionViewEvent::MegaExplosionViewEvent(size_t i, size_t j) : ViewEvent(VIEW_EVENT_MEGA_EXPLOSION), i(i), j(j){}
CreateTokenViewEvent::CreateTokenViewEvent(size_t i, size_t j, Token* token, std::string token_file_name) :
	ViewEvent(VIEW_EVENT_CREATE_TOKEN), i(i), j(j), token(token), token_file_name(token_file_name){}
MoveTokenViewEvent::MoveTokenViewEvent(size_t i, size_t j, Token* token) : ViewEvent(VIEW_EVENT_MOVE_TOKEN), token(token), i(i), j(j){}
MoveTokenDirectionViewEvent::MoveTokenDirectionViewEvent(Directions direction, Token* token) : 
	ViewEvent(VIEW_EVENT_MOVE_TOKEN_DIRECTION), direction(direction), token(token){}
InitGameBoardViewEvent::InitGameBoardViewEvent(size_t width, size_t height) : ViewEvent(VIEW_EVENT_INIT_GAME_BOARD), width(width), height(height){}