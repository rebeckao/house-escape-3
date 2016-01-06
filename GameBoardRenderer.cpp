#include "GameBoardRenderer.h"
#include "View.h"

float pi_by_2 = 1.57079f;


GameBoardRenderer::GameBoardRenderer(){}

GameBoardRenderer::~GameBoardRenderer(){
	clearGameBoardData();
	glDeleteProgram(shader_program);
	glDeleteBuffers(1,&vertex_buffer_object);
	glDeleteBuffers(1, &element_buffer);
	glDeleteVertexArrays(1, &vertex_array_object);
}

void GameBoardRenderer::clearGameBoardData(){
	for (size_t i = 0; i < game_board_tile_data.size(); i++){
		for (size_t j = 0; j < game_board_tile_data[i].size(); j++){
			delete game_board_tile_data[i][j];
		}
	}
	game_board_tile_data.clear();

	for (size_t i = 0; i < game_board_token_data.size(); i++){
		for (size_t j = 0; j < game_board_token_data[i].size(); j++){
			for (size_t k = 0; k < game_board_token_data[i][j].size(); k++){
				delete game_board_token_data[i][j][k];
			}
		}
	}
	game_board_token_data.clear();
	tile_to_position.clear();
	token_to_graphic_tile.clear();
}

GameBoardRenderer::GameBoardRenderer(std::string vertex_shader_path, std::string fragment_shader_path, Camera* input_camera)
				  :camera(input_camera){
	//load, compile and link shader program for game board
	vertex_shader = LoadShader(std::string("GLSL/game_board_vertex_shader.txt"), GL_VERTEX_SHADER);
	fragment_shader = LoadShader(std::string("GLSL/game_board_fragment_shader.txt"), GL_FRAGMENT_SHADER);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);

	glBindFragDataLocation(shader_program, 0, "out_color");
	glAttachShader(shader_program, fragment_shader);

	glLinkProgram(shader_program);
	CheckLinkStatus(shader_program);

	glDetachShader(shader_program, vertex_shader);
	glDetachShader(shader_program, fragment_shader);
	
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	//once shader is linked, get handle to model view projection matrix
	mvp_uniform_attrib = glGetUniformLocation(shader_program, "mvp");

	//create vertex buffer object on the graphics card
	float tile_vertices[] = {
		-0.5, -0.5, 0, 1,
		 0.5, -0.5, 1, 1,
		 0.5,  0.5, 1, 0,
		-0.5,  0.5, 0, 0
	};

	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tile_vertices), tile_vertices, GL_STATIC_DRAW);

	//create corresponding vertex array object on the graphics card
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);

	GLuint elements[] = {
		0, 1, 2,
		0, 2, 3,
	};

	element_buffer;
	glGenBuffers(1, &element_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	GLint vertex_position_attrib = glGetAttribLocation(shader_program, "vertex_position");
	glVertexAttribPointer(vertex_position_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(vertex_position_attrib);

	GLint texture_coordinate_attrib = glGetAttribLocation(shader_program, "texture_coordinate");
	glVertexAttribPointer(texture_coordinate_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(texture_coordinate_attrib);

	initTilePaths();
}

void GameBoardRenderer::initGameBoardData(size_t width, size_t height){
	clearGameBoardData();
	for (size_t i = 0; i < height; i++){
		game_board_tile_data.push_back(std::vector<GraphicTile*>());
		for (size_t j = 0; j < width; j++){
			GraphicTile* new_tile = new GraphicTile();
			updateTileToPosition(new_tile, i, j);
			game_board_tile_data[i].push_back(new_tile);
		}
	}

	for (size_t i = 0; i < height; i++){
		game_board_token_data.push_back(std::vector<std::vector<GraphicTile*>>());
		for (size_t j = 0; j < width; j++){
			game_board_token_data[i].push_back(std::vector<GraphicTile*>());
		}
	}
}

void GameBoardRenderer::initTilePaths(){
	DIR *directory;
	dirent *directory_entry;
	std::string relative_path("Graphics/Tiles/");
	if ((directory = opendir(relative_path.c_str())) != NULL) {
		while ((directory_entry = readdir(directory)) != NULL) {
			if (!std::strcmp(directory_entry->d_name, ".") || !std::strcmp(directory_entry->d_name, "..")){
				//do nothing
			}
			else{
				std::string file_name(directory_entry->d_name);
				tile_paths[classifyTile(file_name)].push_back(relative_path + directory_entry->d_name);
			}
		}
		closedir(directory);
		random_tile_distributions[TileClassification::UNEXPLORED] = std::uniform_int_distribution<>(0, tile_paths[TileClassification::UNEXPLORED].size() - 1);
		random_tile_distributions[TileClassification::NO_EXIT] = std::uniform_int_distribution<>(0, tile_paths[TileClassification::NO_EXIT].size() - 1);
		random_tile_distributions[TileClassification::ONE_EXIT] = std::uniform_int_distribution<>(0, tile_paths[TileClassification::ONE_EXIT].size() - 1);
		random_tile_distributions[TileClassification::TWO_EXITS_ADJACENT] = std::uniform_int_distribution<>(0, tile_paths[TileClassification::TWO_EXITS_ADJACENT].size() - 1);
		random_tile_distributions[TileClassification::TWO_EXITS_APART] = std::uniform_int_distribution<>(0, tile_paths[TileClassification::TWO_EXITS_APART].size() - 1);
		random_tile_distributions[TileClassification::THREE_EXITS] = std::uniform_int_distribution<>(0, tile_paths[TileClassification::THREE_EXITS].size() - 1);
		random_tile_distributions[TileClassification::FOUR_EXITS] = std::uniform_int_distribution<>(0, tile_paths[TileClassification::FOUR_EXITS].size() - 1);
		random_tile_generator.seed(1);		
	}
}

void GameBoardRenderer::renderGameBoard(){
	glUseProgram(shader_program);	
	glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_object);
	glBindVertexArray(vertex_array_object);
	camera->updateViewProjectionFromInput();

	for (size_t i = 0; i < game_board_tile_data.size(); i++){
		for (size_t j = 0; j < game_board_tile_data[i].size(); j++){
			renderTile(*game_board_tile_data[i][j], i, j);
		}
	}

	for (size_t i = 0; i < game_board_token_data.size(); i++){
		for (size_t j = 0; j < game_board_token_data[i].size(); j++){
			for (size_t k = 0; k < game_board_token_data[i][j].size(); k++){
				renderToken(*game_board_token_data[i][j][k], i, j);
			}
		}
	}	
}

void GameBoardRenderer::renderTile(GraphicTile&  current_tile, const size_t & i, const size_t & j){
	if (current_tile.texture == 0){ return; }
	glm::mat4 model_matrix =glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3((float)(j*tile_scale), -(float)(i*tile_scale), 0.1f)),
		glm::vec3(tile_scale, tile_scale, tile_scale)), current_tile.rotation, glm::vec3(0, 0, 1.0f));
	glm::mat4 model_view_projection = camera->projection_matrix*camera->view_matrix*model_matrix;
	glUniformMatrix4fv(mvp_uniform_attrib, 1, GL_FALSE, &model_view_projection[0][0]);
	glBindTexture(GL_TEXTURE_2D, current_tile.texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GameBoardRenderer::renderToken(GraphicTile&  current_tile, const size_t & i, const size_t & j){
	if (current_tile.texture == 0){ return; }
	glm::mat4 model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3((float)(j*tile_scale), -(float)(i*tile_scale), 0.2f)),
		glm::vec3(token_scale, token_scale, token_scale));
	glm::mat4 model_view_projection = camera->projection_matrix*camera->view_matrix*model_matrix;
	glUniformMatrix4fv(mvp_uniform_attrib, 1, GL_FALSE, &model_view_projection[0][0]);
	glBindTexture(GL_TEXTURE_2D, current_tile.texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


GraphicTile* GameBoardRenderer::getGraphicTile(size_t i, size_t j){
	return game_board_tile_data[i][j];
}

GraphicTile* GameBoardRenderer::getToken(size_t i, size_t j){
	return game_board_token_data[i][j][0];
}

void GameBoardRenderer::createToken(size_t i, size_t j, Token* token,std::string token_file_name){
	if (positionInBounds(i,j)){
		std::string file_path = std::string("Graphics/Tokens/") + token_file_name;
		GraphicTile* new_tile = new GraphicTile(file_path);
		game_board_token_data[i][j].push_back(new_tile);
		token_to_graphic_tile[token] = new_tile;
		updateTileToPosition(new_tile, i, j);
	}
}

void GameBoardRenderer::moveTokenGraphicTile(GraphicTile* token, size_t i, size_t j){
	if (positionInBounds(i,j)){
		std::vector<size_t> position = tile_to_position[token];
		int token_position = -1;
		for (size_t k = 0; k < game_board_token_data[position[0]][position[1]].size(); k++){
			if (token == game_board_token_data[position[0]][position[1]][k]){
				token_position = k;
				break;
			}
		}
		if (token_position > -1){
			game_board_token_data[i][j].push_back(game_board_token_data[position[0]][position[1]][token_position]);
			updateTileToPosition(token, i, j);
			game_board_token_data[position[0]][position[1]].erase(game_board_token_data[position[0]][position[1]].begin() + token_position);
		}		
	}
}

void GameBoardRenderer::exploreTile(const size_t & i, const size_t & j, const std::vector<bool> & room_exits){
	GraphicTile* current_tile = game_board_tile_data[i][j];
	TileClassification tile_type = classifyTile(room_exits);
	std::string tile_path = tile_paths[tile_type][random_tile_distributions[tile_type](random_tile_generator)];
	float rotation = relativeRotation(roomExitsToUnsignedChar(room_exits),roomExitsToUnsignedChar(tilePathToRoomExits(tile_path)));
	current_tile->updateTexture(tile_path, rotation);
}

std::vector<bool> GameBoardRenderer::tilePathToRoomExits(const std::string & tile_path){
	std::vector<bool> room_exits(4, false);
	size_t file_extension_position = tile_path.find_last_of('.');
	for (size_t i = 0; i < 4; i++){
		if (tile_path[file_extension_position - 4 + i] == '1'){ room_exits[i] = true; }
	}
	return room_exits;
}

float GameBoardRenderer::relativeRotation(unsigned char target, unsigned char room_exits){
	for (size_t i = 0; i < 4; i++){
		if (target == room_exits){
			return static_cast<float>(i)*pi_by_2;
		}
		room_exits = rotateLeft4bits(room_exits,1);
	}
}



bool GameBoardRenderer::positionInBounds(const size_t & i, const size_t & j){
	if (i < game_board_tile_data.size() && j < game_board_tile_data[0].size()){
		return true;
	}
	return false;
}

void GameBoardRenderer::updateTileToPosition(GraphicTile* & new_tile, const size_t & i, const size_t & j){
	std::vector<size_t> position;
	position.push_back(i);
	position.push_back(j);
	tile_to_position[new_tile] = position;
}

GraphicTile::GraphicTile(){
	std::string default_path = ("Graphics/Tiles/unexplored.bmp");
	setTexture(default_path);
}

GraphicTile::GraphicTile(const std::string& image_path){
	setTexture(image_path);
}

GraphicTile::~GraphicTile(){
	glDeleteTextures(1, &texture);
}

void GraphicTile::updateTexture(const std::string & image_path, const float & input_rotation){
	glDeleteTextures(1, &texture);
	setTexture(image_path);
	rotation = input_rotation;
}

void GraphicTile::setTexture(const std::string & image_path){
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	SDL_Surface* texture_image = SDL_LoadBMP(image_path.c_str());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_image->w, texture_image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_image->pixels);
	SDL_FreeSurface(texture_image);

	float anisotropy = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

unsigned char GameBoardRenderer::rotateLeft4bits(unsigned char value, unsigned char rotations){
	unsigned char four_bit_value = value & 15; // 15 = 00001111
	if (rotations == 0 || !(rotations < 4)) return value;
	return ((four_bit_value << rotations) & 15) | (four_bit_value >> 4 - rotations);
}

unsigned char GameBoardRenderer::rotateRight4bits(unsigned char value, unsigned char rotations){
	unsigned char four_bit_value = value & 15; // 15 = 00001111
	if (rotations == 0 || !(rotations < 4)) return value;
	return (four_bit_value >> rotations) | ((four_bit_value << 4 - rotations) & 15);
}

TileClassification GameBoardRenderer::classifyTile(const std::string &  file_name){
	size_t file_extension_position = file_name.find_last_of('.');
	if (file_name.compare(0,file_extension_position,"unexplored") == 0){
		return TileClassification::UNEXPLORED;
	}
	/*
	std::vector<bool> room_exits(4, false);
	for (size_t i = 0; i < 4; i++){
		if (file_name[file_extension_position - 4 + i] == '1'){ room_exits[i] = true; }		
	}
	*/	
	return classifyTile(tilePathToRoomExits(file_name));
}

TileClassification GameBoardRenderer::classifyTile(const std::vector<bool> & room_exits){
	int number_of_exits = 0;
	for (size_t i = 0; i < room_exits.size(); i++){
		if (room_exits[i]){
			++number_of_exits;
		}
	}
	if (number_of_exits == 0){
		return TileClassification::NO_EXIT;
	}
	else if (number_of_exits == 1){
		return TileClassification::ONE_EXIT;
	}
	else if (number_of_exits == 3){
		return TileClassification::THREE_EXITS;
	}
	else if (number_of_exits == 4){
		return TileClassification::FOUR_EXITS;
	}
	else{
		unsigned char room_exits_as_u_char = roomExitsToUnsignedChar(room_exits);
		for (size_t i = 0; i < 4; i++){
			if (room_exits_as_u_char == 12){ // 12 = 1100, an adjacent room
				return TileClassification::TWO_EXITS_ADJACENT;
			}
			room_exits_as_u_char = rotateRight4bits(room_exits_as_u_char, 1);
		}
		return TileClassification::TWO_EXITS_APART;
	}
}

unsigned char GameBoardRenderer::roomExitsToUnsignedChar(const std::vector<bool> & room_exits){
	unsigned char room_exits_as_u_char = 0;
	for (size_t i = 0; i < 4; i++){
		room_exits_as_u_char <<= 1;
		if (room_exits[i]){
			room_exits_as_u_char += 1;
		}
	}
	return room_exits_as_u_char;
}