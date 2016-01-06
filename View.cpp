#include "View.h"

View::View(Model& input_model):model(input_model){
	game_board_renderer = new GameBoardRenderer(std::string("GLSL/game_board_vertex_shader.txt"),
											std::string("GLSL/game_board_fragment_shader.txt"),
											&camera);
	initEvents();
	glEnable(GL_DEPTH_TEST);
}

View::~View(){
	delete game_board_renderer;
}

void View::render(SDL_Window* window){

	//std::vector<double> background_color = model.getColor();	
	//glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	handleEvents();
	game_board_renderer->renderGameBoard();

	glFlush();
	SDL_GL_SwapWindow(window);
}

void View::handleEvents(){
	for (size_t i = 0; i < model.view_event_queue.size(); i++){		
		callEvent(model.view_event_queue[i]);
	}
	clearEvents();
}

void View::callEvent(ViewEvent*& view_event){
	(this->*(event_map[view_event->event_ID]))(view_event);
}

void View::clearEvents(){
	for (size_t i = 0; i < model.view_event_queue.size(); i++){
		delete model.view_event_queue[i];
	}
	model.view_event_queue.clear();
}

GLuint LoadShader(std::string shader_file_name, GLenum shader_type){
	std::ifstream my_stream(shader_file_name);
	std::string contents = std::string((std::istreambuf_iterator<char>(my_stream)), std::istreambuf_iterator<char>());
	const GLchar* shader_code = contents.c_str();
	my_stream.close();

	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_code, nullptr);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE){
		std::cout << "shader compile error in: " << shader_file_name << std::endl;
		char buffer[512];
		glGetShaderInfoLog(shader, 512, nullptr, buffer);
		std::cout << buffer << std::endl;
	}
	return shader;
}

void CheckLinkStatus(GLuint& shader_program){
	GLint link_status;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &link_status);
	if (link_status != GL_TRUE){
		std::cout << "shader linker error in View constructor" << std::endl;
		char buffer[512];
		glGetProgramInfoLog(shader_program, 512, nullptr, buffer);
	}
}

//
//Event functions
//
void View::initEvents(){
	void(View::* member_function_pointer)(ViewEvent*);

	member_function_pointer = &View::exploreTile;
	event_map[VIEW_EVENT_EXPLORE_TILE] = member_function_pointer;

	member_function_pointer = &View::megaExplosion;
	event_map[VIEW_EVENT_MEGA_EXPLOSION] = member_function_pointer;

	member_function_pointer = &View::createToken;
	event_map[VIEW_EVENT_CREATE_TOKEN] = member_function_pointer;

	member_function_pointer = &View::moveToken;
	event_map[VIEW_EVENT_MOVE_TOKEN] = member_function_pointer;

	member_function_pointer = &View::moveTokenInDirection;
	event_map[VIEW_EVENT_MOVE_TOKEN_DIRECTION] = member_function_pointer;

	member_function_pointer = &View::initGameBoard;
	event_map[VIEW_EVENT_INIT_GAME_BOARD] = member_function_pointer;	
}

void View::exploreTile(ViewEvent* tile_explored_event){
	TileExploredViewEvent* current_event = dynamic_cast<TileExploredViewEvent*>(tile_explored_event);
	if (game_board_renderer->positionInBounds(current_event->i,current_event->j)){
		game_board_renderer->exploreTile(current_event->i, current_event->j,current_event->room_exits);
	}	
}

void View::megaExplosion(ViewEvent* mega_explosion_event){
	MegaExplosionViewEvent* current_event = dynamic_cast<MegaExplosionViewEvent*>(mega_explosion_event);
	std::cout << "IT'S A BOMB!" << std::endl;
	std::cout << current_event->i << " " << current_event->j << std::endl;
}

void View::createToken(ViewEvent* create_token_event){
	CreateTokenViewEvent* current_event = dynamic_cast<CreateTokenViewEvent*>(create_token_event);
	game_board_renderer->createToken(current_event->i, current_event->j, current_event->token,current_event->token_file_name);
}

void View::moveToken(ViewEvent* move_token_event){
	MoveTokenViewEvent* current_event = dynamic_cast<MoveTokenViewEvent*>(move_token_event);
	GraphicTile* current_token = game_board_renderer->token_to_graphic_tile[current_event->token];
	game_board_renderer->moveTokenGraphicTile(current_token, current_event->i, current_event->j);
}

void View::moveTokenInDirection(ViewEvent* move_token_direction_event){
	MoveTokenDirectionViewEvent* current_event = dynamic_cast<MoveTokenDirectionViewEvent*>(move_token_direction_event);
	GraphicTile* current_token = game_board_renderer->token_to_graphic_tile[current_event->token];
	std::vector<size_t> position = game_board_renderer->tile_to_position[current_token];
	GameBoard::updatePosition(position, current_event->direction);
	game_board_renderer->moveTokenGraphicTile(current_token, position[0], position[1]);
}

void View::initGameBoard(ViewEvent* init_game_board_event){
	InitGameBoardViewEvent* current_event = dynamic_cast<InitGameBoardViewEvent*>(init_game_board_event);
	game_board_renderer->initGameBoardData(current_event->width, current_event->height);
}