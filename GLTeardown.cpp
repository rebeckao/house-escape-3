#include "GLTeardown.h"

/*
Call when program is done
*/
void tearDownEverything(SDL_Window*& window){
	SDL_GL_DeleteContext(window);
	SDL_DestroyWindow(window);

	// Clean up SDL
	SDL_Quit();
}