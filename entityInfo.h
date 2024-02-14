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
	unsigned short int type = 0;
	bool Onscreen = false;
	bool launchBullet = false;
	bool gameover = false;
}Plane;

typedef struct _Bullet
{
	SDL_Rect rect = {};
	Vector CollisionWithWall = {};
	Vector speed = { 0 };
	bool Onscreen = false;
}Bullet;