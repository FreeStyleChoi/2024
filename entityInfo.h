#pragma once

#include "main.h"
#include "vector.h"


struct Background
{
	SDL_Rect rect = {};
	Vector speed{};
	bool Onscreen = false;
//	Background& Init(const char* filename, SDL_Texture* Tex ,uint32_t count);
};

struct Plane
{
	SDL_Rect rect = {};
	Vector speed = {};
	Vector CollisionWithWall = {};
	bool Onscreen = false;
//	Plane& Init();
};

struct Bullet
{
	SDL_Rect rect = {};
	Vector CollisionWithWall = {};
	Vector speed = { 0 };
	bool Onscreen = false;
//	Bullet& Init();
};

void InitBackground(Background* Ocean, SDL_Texture* OceanTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer);
Plane* InitUser(Plane* User, SDL_Texture* UserTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer);
void InitEnemy(Plane* Enemy, SDL_Texture* EnemyTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer);
void InitUserBullet(Plane* User, Bullet* UserBullet, SDL_Texture* UserBulletTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer);
void InitEnemyBullet(Plane* User, Bullet* EnemyBullet, SDL_Texture* EnemyBulletTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer);
void InitMainBackground(Background* mainbg, SDL_Texture* mainbgTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer);
