#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include "ViewEvent.h"
#include "Token.h"
#include "Tile.h"
#include <vector>

class Model{
private:
	std::vector<double> screen_color;
	
public:
	std::vector<ViewEvent*> view_event_queue;
	Token* current_token;
	size_t token_x = 0, token_y = 0;

	Model();
	void setColor(double r, double g, double b, double alpha);
	void add_event();
	std::vector<double> getColor() const;
	void move(int i);
};

void timeStepModel(Model& model, double timeStep);

class TileUpdateEvent :public ViewEvent{

};












//class Monster: public Character{
//	void move(int direction);
//};

class Item{
	std::string name;
	//Model-events
	unsigned int weight;
	unsigned int value;


};

/*
class Player{
GameBoard& current_gameboard;
std::vector<Token&> owned_tokens;
Token& active_token;

void move_token_north(Token& active_token);
void move_token_east(Token& active_token);
void move_token_south(Token& active_token);
void move_token_west(Token& active_token);
void search();
};

*/

#endif