#include "GameBoard.h"
#include <algorithm>

GameBoard::GameBoard():GameBoard("Maps/map2.txt") {}

GameBoard::~GameBoard(){
	cleanupTiles();
	cleanupTokens();
}

void GameBoard::cleanupTiles(){
	for (size_t i = 0; i < tile_matrix.size(); i++){
		for (size_t j = 0; j < tile_matrix[i].size(); j++){
			delete tile_matrix[i][j];
		}
	}
	tile_matrix.clear();
}

void GameBoard::cleanupTokens(){
	for (size_t i = 0; i < token_matrix.size(); i++){
		for (size_t j = 0; j < token_matrix[i].size(); j++){
			for (size_t k = 0; k < token_matrix[i][j].size(); k++){
				delete token_matrix[i][j][k];
			}
		}
	}
	token_locations.clear();
}

GameBoard::GameBoard(int rows, int cols) {
	std::vector<Tile*> row_vector;
	for (int row = 0; row < rows; ++row) {
		row_vector.clear();
		for (int col = 0; col < cols; ++col) {
			row_vector.push_back(new Tile());
		}
		tile_matrix.push_back(row_vector);
	}
}

GameBoard::GameBoard(std::string in_file_path) {
	std::ifstream infile;
	int rows;
	int cols;
	bool a;
	std::vector<Tile*> row_vector;
	std::vector<bool> file_exits;

	infile.open(in_file_path);
	if (!infile.is_open()){
		std::cout << "Error in GameBoard: could not open map" << std::endl;
	}
	else{
		infile >> rows;
		infile >> cols;
		for (int i = 0; i < rows; ++i) {
			row_vector.clear();
			for (int j = 0; j < cols; ++j) {
				file_exits.clear();
				for (int k = 0; k < 4; ++k) {
					infile >> a;
					file_exits.push_back(a);

				}
				row_vector.push_back(new Tile(file_exits));

			}
			tile_matrix.push_back(row_vector);
		}
		token_matrix = std::vector<std::vector<std::vector<Token*>>>(rows);
		for (int l = 0; l < rows; ++l) {
			token_matrix[l].resize(cols);
		}
	}
}

void GameBoard::insert_token(Token* new_token, const size_t row, const size_t col) {
	std::vector<size_t> index_vector = { row, col };
	token_locations[new_token] = index_vector;
	token_matrix[row][col].push_back(new_token);
}

void GameBoard::destroy_token(Token* old_token) {
	//should probably call the token destructor and let view know a token was destroyed
}

bool GameBoard::legalMove(Token* current_token, const Directions & direction){
	std::vector<size_t> position = token_locations[current_token];
	if (exitExists(position,direction)){
		position = updatePosition(position, direction);
		if (positionInBounds(position)){
			return true;
		}
	}
	return false;
}

bool GameBoard::positionInBounds(const std::vector<size_t>& position){//assumes non-empty rectangular matrix
	return position[0] < tile_matrix.size() && position[1] < tile_matrix[0].size();	
}

bool GameBoard::exitExists(const std::vector<size_t>& position, const Directions & direction) {
	return tile_matrix[position[0]][position[1]]->exits[direction] == 1;
}

bool GameBoard::move_token(Token* current_token, Directions direction) {

	if (legalMove(current_token, direction)){
		std::vector<size_t> new_coordinates = token_locations[current_token];
		new_coordinates = updatePosition(new_coordinates, direction);

		//remove token from token_matrix
		std::vector<Token*>* current_token_vector = &token_matrix[token_locations[current_token][0]][token_locations[current_token][1]];
		current_token_vector->erase(std::remove(current_token_vector->begin(), current_token_vector->end(), current_token), current_token_vector->end());

		//update hash-map
		token_locations.erase(current_token);
		token_locations[current_token] = new_coordinates;

		//insert token at new location in token_matrix
		current_token_vector = &token_matrix[token_locations[current_token][0]][token_locations[current_token][1]];
		current_token_vector->push_back(current_token);
		return true;
	}
	return false;
}

std::vector<size_t>& GameBoard::updatePosition(std::vector<size_t>& position, const Directions& direction){
	switch (direction) {
	case GAMEBOARD_EAST:
		++position[1];
		break;
	case GAMEBOARD_NORTH:
		--position[0];
		break;
	case GAMEBOARD_SOUTH:
		++position[0];
		break;
	case GAMEBOARD_WEST:
		--position[1];
		break;
	}
	return position;
}