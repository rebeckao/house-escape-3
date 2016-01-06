#include "Token.h"
#include <string>
Token::Token(){
	name = "Mr. Placeholder";
	inventory = new Inventory;
}
Token::~Token(){
	delete inventory;
}

std::string Token::getName() const{ return name; }

MonsterToken::MonsterToken(Model& model):model(model){}

SpiderMonster::SpiderMonster(const int & input_action_threshold, const int & input_monster_speed, Model& model) : MonsterToken(model){
	action_threshold = input_action_threshold;
	monster_speed = input_monster_speed;
}

void SpiderMonster::action(){
	action_progression += monster_speed;
	if (action_progression >= action_threshold){
		action_progression = 0;
		move();
		attack();
	}
}

void SpiderMonster::move(){
	std::vector<size_t> player_position = model.gameBoard->token_locations[model.active_player_token];
	std::vector<size_t> monster_position = model.gameBoard->token_locations[this];
	if (monster_position[0] < player_position[0] && model.gameBoard->legalMove(this, Directions::GAMEBOARD_SOUTH)){
		model.moveToken(&MoveTokenModelEvent(this, GAMEBOARD_SOUTH));
	}else if (monster_position[1] < player_position[1] && model.gameBoard->legalMove(this, Directions::GAMEBOARD_EAST)){
		model.moveToken(&MoveTokenModelEvent(this, GAMEBOARD_EAST));
	}
	else if (monster_position[0] > player_position[0] && model.gameBoard->legalMove(this, Directions::GAMEBOARD_NORTH)){
		model.moveToken(&MoveTokenModelEvent(this, GAMEBOARD_NORTH));
	}
	else if (monster_position[1] > player_position[1] && model.gameBoard->legalMove(this, Directions::GAMEBOARD_WEST)){
		model.moveToken(&MoveTokenModelEvent(this, GAMEBOARD_WEST));
	}else{
		moveClockwise();
	}
}

void SpiderMonster::moveClockwise(){
	if (model.gameBoard->legalMove(this, Directions::GAMEBOARD_EAST)){
		model.moveToken(&MoveTokenModelEvent(this, GAMEBOARD_EAST));
	}
	else if (model.gameBoard->legalMove(this, Directions::GAMEBOARD_SOUTH)){
		model.moveToken(&MoveTokenModelEvent(this, GAMEBOARD_SOUTH));
	}
	else if (model.gameBoard->legalMove(this, Directions::GAMEBOARD_WEST)){
		model.moveToken(&MoveTokenModelEvent(this, GAMEBOARD_WEST));
	}
	else if (model.gameBoard->legalMove(this, Directions::GAMEBOARD_NORTH)){
		model.moveToken(&MoveTokenModelEvent(this, GAMEBOARD_NORTH));
	}
}

void SpiderMonster::attack(){
	std::vector<size_t> player_position = model.gameBoard->token_locations[model.active_player_token];
	std::vector<size_t> monster_position = model.gameBoard->token_locations[this];
	if (monster_position == player_position){
		std::cout << "The spider attacks! But your carrot saves you." << std::endl;
	}
}