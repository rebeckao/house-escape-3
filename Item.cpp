#include "Item.h"

Item::Item()
{
}

Item::~Item()
{
}

std::string Item::getDescription(){
	return description;
}


KeyItem::KeyItem() {
	name = "Key of Doom";
	value = 500;
	weight = 0.1;
}


void KeyItem::useEffect(Model &model) {
	if (model.gameBoard->token_locations[model.active_player_token][0] == 6 &&
		model.gameBoard->token_locations[model.active_player_token][1] == 3 &&
		std::find_if(model.active_player_token->inventory->inventory_items.begin(), model.active_player_token->inventory->inventory_items.end(),
		[](Item* item){return item->name == "Key of Doom"; }) != model.active_player_token->inventory->inventory_items.end()){

		auto iterator = 
			std::find_if(model.active_player_token->inventory->inventory_items.begin(), model.active_player_token->inventory->inventory_items.end(),
			[](Item* item) {return item->name == "Key of Doom"; });

		std::cout << "You escape The House! " << std::endl;
		std::cout << "But the memories of it's planloesning will haunt you forever..." << std::endl;
		std::cout << "Press ESC to quit..." << std::endl;
	}else{
		std::cout << "This is not the locked room! Where was it again...?" << std::endl;
	}
}

PotionItem::PotionItem() {
	name = "Mysterious Potion";
	value = 10;
	weight = 0.5;
}

void PotionItem::useEffect(Model &model) {
	std::cout << "You feel hazy as you drink the potion. You see what looks like a fireplace, no, balcony.. or a toilet? This must be where the key is!" << std::endl;
	if (model.gameBoard->tile_matrix[2][2]->explored == false)
	{
		model.gameBoard->tile_matrix[2][2]->explored = true;
	}
}

CarrotItem::CarrotItem(){
	name = "Carrot";
	weight = 0.1;
	description = "Orange and sleek, the vegetable hums with power in your hand. No wonder rabbits are such fierce warriors."; 
}

void CarrotItem::useEffect(Model &model){
	std::cout << getDescription() << std::endl;
}