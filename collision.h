#pragma once
#include <SDL.h>
#include "vector.h"

bool collision(SDL_Rect A, SDL_Rect B);

Vector wallCollision(SDL_Rect A, int window_W, int window_H);