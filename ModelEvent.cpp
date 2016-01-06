#include "ModelEvent.h"

ModelEvent::ModelEvent()
{
}
ModelEvent::ModelEvent(size_t input_event_ID) :event_ID(input_event_ID) 
{
}
ModelEvent::~ModelEvent()
{
}

MoveTokenModelEvent::MoveTokenModelEvent(Token* token, Directions direction) : ModelEvent(MODEL_EVENT_MOVE_TOKEN), token(token), direction(direction) {};
InputDirectionModelEvent::InputDirectionModelEvent(Directions direction) : ModelEvent(MODEL_INPUT_DIRECTION), direction(direction) {};
InsertTokenModelEvent::InsertTokenModelEvent(Token* token, size_t i, size_t j) : ModelEvent(MODEL_EVENT_CREATE_TOKEN), token(token), i(i), j(j) {};
ExploreTileModelEvent::ExploreTileModelEvent(Tile* tile, size_t i, size_t j) : ModelEvent(MODEL_EVENT_EXPLORE_TILE), tile(tile), i(i), j(j) {};
InputInventoryDirectionModelEvent::InputInventoryDirectionModelEvent(Directions direction) : ModelEvent(MODEL_INPUT_INVENTORY_DIRECTION), direction(direction) {};
InputToggleInventoryModelEvent::InputToggleInventoryModelEvent() : ModelEvent(MODEL_INPUT_INVENTORY_TOGGLE){};
UseCurrentItemModelEvent::UseCurrentItemModelEvent() : ModelEvent(MODEL_EVENT_USE_CURRENT_ITEM){};
FindKeyModelEvent::FindKeyModelEvent(Token* player_token) : ModelEvent(MODEL_EVENT_FIND_KEY), player_token(player_token) {};