#pragma once

typedef struct _Background
{
	SDL_Rect rect = {};
	Vector speed{};
	bool Onscreen = false;
}Background;

typedef struct _Plane
{
	SDL_Rect rect = {};
	Vector speed = {};
	Vector CollisionWithWall = {};
	unsigned short int health = 100;
	bool Onscreen = false;
	bool launchBullet = false;
	bool gameover = false;
}Plane;

typedef struct _Bullet
{
	SDL_Rect rect = {};
	Vector CollisionWithWall = {};
	double speed = 0.7;
	bool Onscreen = false;
}Bullet;