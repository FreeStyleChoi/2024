#include "collision.h"

bool collision(SDL_Rect A, SDL_Rect B)
{
	bool Cx = A.x + A.w >= B.x && A.x <= B.x + B.w;
	bool Cy = A.y + A.h >= B.y && A.y <= B.y + B.h;
	bool resault = false;

	if (Cx && Cy)
	{
		resault = true;
	}

	return resault;
}

Vector wallCollision(SDL_Rect A, int window_W, int window_H)
{
	Vector result = { 0 };
	if (A.y <= 0)
	{
		result.y = -1;
	}
	else if (A.y >= window_H - A.h)
	{
		result.y = 1;
	}
	if (A.x <= 0)
	{
		result.x = -1;
	}
	else if (A.x >= window_W - A.w)
	{
		result.x = 1;
	}
	return result;
}
