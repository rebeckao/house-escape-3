#ifndef GLTEARDOWN_H
#define GLTEARDOWN_H
#include <iostream>
#include "SDL.h"
#include "glew.h"

/*
Call when program is done
*/
void tearDownEverything(SDL_Window*& window);

#endif