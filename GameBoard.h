#ifndef GAMEBOARD_H
#define GAMEBOARD_H

enum Directions;
class GameBoard;
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include "Token.h"
#include "Tile.h"
#include "ModelEvent.h"

class GameBoard {
public:
	GameBoard();
	GameBoard(int rows, int cols);
	GameBoard(std::string in_file_stream);
	
	~GameBoard();

	std::vector<std::vector<Tile*>> tile_matrix;
	std::vector<std::vector<std::vector<Token*>>> token_matrix;
	std::unordered_map<Token*, std::vector<size_t>> token_locations;

	void insert_token(Token* new_token, const size_t row, const size_t col);
	void destroy_token(Token* old_token);
	bool move_token(Token* current_token, Directions direction);

	bool exitExists(const std::vector<size_t>& position, const Directions & direction) const;
	bool positionInBounds(const std::vector<size_t>& position) const;
	bool legalMove(Token* current_token, const Directions & direction);

	void cleanupTiles();
	void cleanupTokens();
	static std::vector<size_t>& updatePosition(std::vector<size_t>& position, const Directions& direction);
};

#endif