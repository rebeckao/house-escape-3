//#include <tclDecls.h>
#include "Model.h"
#include <algorithm>

void Model::add_event(){
	TileExploredEvent* my_event = new TileExploredEvent(1, 1, std::vector<bool>(4,true));
	view_event_queue.push_back(my_event);

	MegaExplosionEvent* my_other_event = new MegaExplosionEvent(0, 0);
	view_event_queue.push_back(my_other_event);
	
	CreateTokenEvent* my_third_event = new CreateTokenEvent(3, 3, current_token);
	view_event_queue.push_back(my_third_event);
}

void Model::move(int i){
	if (i == 0){
		token_y--;
		MoveTokenEvent* current_event = new MoveTokenEvent(token_y, token_x, current_token);
		view_event_queue.push_back(current_event);
		TileExploredEvent* other_event = new TileExploredEvent(token_y, token_x, std::vector < bool > {1,0,1,1});
		view_event_queue.push_back(other_event);
	}else if(i == 1){
		token_x++;
		MoveTokenEvent* current_event = new MoveTokenEvent(token_y, token_x, current_token);
		view_event_queue.push_back(current_event);
		TileExploredEvent* other_event = new TileExploredEvent(token_y, token_x, std::vector < bool > {1,1,0,0});
		view_event_queue.push_back(other_event);
	}else if(i == 2){
		token_y++;
		MoveTokenEvent* current_event = new MoveTokenEvent(token_y, token_x, current_token);
		view_event_queue.push_back(current_event);
		TileExploredEvent* other_event = new TileExploredEvent(token_y, token_x, std::vector < bool > {0,0,1,0});
		view_event_queue.push_back(other_event);
	}else if (i == 3){
		token_x--;
		MoveTokenEvent* current_event = new MoveTokenEvent(token_y, token_x, current_token);
		view_event_queue.push_back(current_event);
		TileExploredEvent* other_event = new TileExploredEvent(token_y, token_x, std::vector < bool > {0,1,0,1});
		view_event_queue.push_back(other_event);
	}
}

Model::Model(){
	screen_color = std::vector<double>(4, 1.0);
	current_token = new Token();
}

void Model::setColor(double r, double g, double b, double alpha){
	screen_color[0] = r;
	screen_color[1] = g;
	screen_color[2] = b;
	screen_color[3] = alpha;
}

std::vector<double>  Model::getColor() const{
	return screen_color;
}

void timeStepModel(Model& model, double timeStep){}

//Tile functions
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////




//Gameboard functions
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////
