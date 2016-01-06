#include "GLInit.h"
#include <iostream>
#include "SDL.h"
#include "glew.h"
#include "Model.h"
#include "View.h"
#include "GameControl.h"
#include <fstream>
#include <string>
#include "GLTeardown.h"
#include "ControlManager.h"


void printIntro(){
	std::cout << "Welcome to House Escape 3!" << std::endl;
	std::cout << "You are trapped in The House and must escape." << std::endl;
	std::cout << "You have gotten to the exit room but lack the Secret Key!" << std::endl;
	std::cout << "Find the Secret Key and return to the room you started in." << std::endl;
	std::cout << "Only then may you escape The House." << std::endl;
	std::cout << "Controls:" << std::endl;
	std::cout << "Move camera using WASD and space, use mouse for lookaround" << std::endl;
	std::cout << "Move character using arrow keys" << std::endl;
	std::cout << "Open/Close inventory with I" << std::endl;
	std::cout << "Cycle through items in inventory with left/right arrow" << std::endl;
	std::cout << "Use items in inventory with E button" << std::endl;
	std::cout << "Press ESC to quit game" << std::endl;
	std::cout << "Good luck!" << std::endl;
	std::cout << "Press Enter to continue..." << std::endl;
	std::cin.get();
}


int main(int argc, char* argv[]){

	printIntro();
	
	SDL_Window* main_window;
	init(main_window);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(main_window);	

	//main loop
	Model model;
	View view(model);
	//GameControl control(model,view);
	//BoardControl control(model, view);
	ControlManager control(model, view);
	
	const double dt = 10;
	double accumulatedTime = 0.0;
	double lastTime = SDL_GetTicks();
	double currentTime;

	/*
	//Example on resizing window:
	SDL_SetWindowSize(main_window, 1024, 1024);
	SDL_DisplayMode mode;
	SDL_GetWindowDisplayMode(main_window, &mode);
	glViewport(0, 0, mode.w, mode.h);
	*/
	
	while (model.game_is_running){

		currentTime = SDL_GetTicks();
		accumulatedTime += currentTime - lastTime;
		lastTime = currentTime;		

		control.handleInput();

		while (accumulatedTime>dt){
			timeStepModel(model, dt);
			accumulatedTime -= dt;
		}
		view.render(main_window);
	}
	
	tearDownEverything(main_window);
	return 0;
}