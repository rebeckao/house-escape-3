#ifndef GAME_BOARD_RENDERER_H
#define GAME_BOARD_RENDERER_H

#include <string>
#include <vector>
#include "Model.h"
#include "SDL.h"
#include "glew.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <dirent.h>
#include <random>

class GraphicTile{
private:
	void setTexture(const std::string & image_path);

public:
	GLuint texture;
	bool explored = false;
	float rotation = 0.0f;

	GraphicTile();
	~GraphicTile();
	GraphicTile(const std::string & image_path);	
	void updateTexture(const std::string & image_path, const float & rotation);
};

enum class TileClassification{
	UNEXPLORED,
	NO_EXIT,
	ONE_EXIT,
	TWO_EXITS_ADJACENT,
	TWO_EXITS_APART,
	THREE_EXITS,
	FOUR_EXITS
};

class GameBoardRenderer{
friend class View;
private:
	GLuint vertex_array_object;
	GLuint vertex_buffer_object;
	GLuint shader_program;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint element_buffer;
	std::vector < std::vector<GraphicTile*> >  game_board_tile_data;
	std::vector < std::vector<std::vector<GraphicTile*>>>  game_board_token_data;
	std::unordered_map<GraphicTile*, std::vector<size_t>>  tile_to_position;
	std::unordered_map<Token*, GraphicTile*>  token_to_graphic_tile;
	GLuint mvp_uniform_attrib;
	float tile_scale = 1.0f;
	float token_scale = 0.3f;
	Camera* camera;
	std::unordered_map<TileClassification,std::vector<std::string>> tile_paths;
	std::unordered_map < TileClassification, std::uniform_int_distribution<>> random_tile_distributions;
	std::mt19937 random_tile_generator;

	void createToken(size_t i, size_t j, Token* token, std::string token_file_name);
	void moveTokenGraphicTile(GraphicTile* token, size_t i, size_t j);
	void updateTileToPosition(GraphicTile* & new_tile, const size_t & i, const size_t & j);
	bool positionInBounds(const size_t & i, const size_t & j);
	void initTilePaths();
	void exploreTile(const size_t & i, const size_t & j, const std::vector<bool> & room_exits);

	TileClassification classifyTile(const std::string & file_name);
	TileClassification classifyTile(const std::vector<bool> & room_exits);
	unsigned char roomExitsToUnsignedChar(const std::vector<bool> & room_exits);
	std::vector<bool> tilePathToRoomExits(const std::string & tile_path);
	float relativeRotation(unsigned char target, unsigned char room_exits);


	GameBoardRenderer();
	~GameBoardRenderer();
	void clearGameBoardData();
	GameBoardRenderer(std::string vertex_shader_path, std::string fragment_shader_path, Camera* input_camera);

	void initGameBoardData(size_t width, size_t height);
	void renderGameBoard();
	void renderTile(GraphicTile&  current_tile, const size_t & i, const size_t & j);
	void renderToken(GraphicTile&  current_tile, const size_t & i, const size_t & j);
	GraphicTile* getGraphicTile(size_t i, size_t j);
	GraphicTile* getToken(size_t i, size_t j);
public: 
	static unsigned char rotateLeft4bits(unsigned char value, unsigned char rotations);
	static unsigned char rotateRight4bits(unsigned char value, unsigned char rotations);
};
#endif