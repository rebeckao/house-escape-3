#ifndef VIEW_CLASS_H
#define VIEW_CLASS_H

#include "SDL.h"
#include "glew.h"
#include <fstream>
#include <string>
#include "Model.h"
#include "Camera.h"
#include "GameBoardRenderer.h"
#include "ViewEvent.h"

class View{
private:
	Model& model;
	GameBoardRenderer* game_board_renderer;
	std::unordered_map<std::size_t, void(View::*)(ViewEvent*)> event_map;

	void handleEvents();
	void callEvent(ViewEvent*& view_event);
	void clearEvents();
	
	//
	//Event functions
	//
	void initEvents();
	void exploreTile(ViewEvent* tile_explored_event);
	void megaExplosion(ViewEvent* mega_explosion_event);
	void createToken(ViewEvent* create_token_event);
	void moveToken(ViewEvent* move_token_event);
	void moveTokenInDirection(ViewEvent* move_token_direction_event);
	void initGameBoard(ViewEvent* init_game_board_event);
	
public:
	Camera camera;

	View(Model& model);
	~View();

	void render(SDL_Window* window);
};

//
//Utitilties
//
GLuint LoadShader(std::string shader_file_name, GLenum shader_type);
void CheckLinkStatus(GLuint& shader_program);

#endif