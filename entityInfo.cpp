#include "main.h"
#include "entityInfo.h"



void InitBackground(Background* Ocean, SDL_Texture* OceanTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer)
{
	if (Ocean == NULL)
	{
		free(Ocean);
		Ocean = NULL;
		OceanTex = NULL;
		Background* Ocean = (Background*)malloc(sizeof(Background) * 2);
		tmpSurface = IMG_Load("./assets/backgrounFinal.png");
		SDL_Texture* OceanTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		SDL_FreeSurface(tmpSurface);
	}
	if (Ocean == NULL)
	{
		free(Ocean);
		OceanTex = NULL;
		printf("Oops\nThere are some error on this game\n(Your computer memory would be full)\n");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		exit(-1);
	}
	Ocean[0].rect.w = WINDOW_W;
	Ocean[0].rect.h = WINDOW_H;
	Ocean[0].rect.x = 0;
	Ocean[0].rect.y = 0;
	Ocean[0].speed.x = 0;
	Ocean[0].speed.y = 0.4;
	Ocean[0].Onscreen = true;
	Ocean[1].rect.w = WINDOW_W;
	Ocean[1].rect.h = WINDOW_H;
	Ocean[1].rect.x = 0;
	Ocean[1].rect.y = 0 - WINDOW_H;
	Ocean[1].speed.x = 0;
	Ocean[1].speed.y = 0.4; // It should be "even" number. If you set it odd number, you will see black line.
	Ocean[1].Onscreen = true;
}

void InitCloud(Background* Cloud, SDL_Texture* CloudTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer)
{
	if (Cloud == NULL)
	{
		free(Cloud);
		Cloud = NULL;
		CloudTex = NULL;
		Background* Cloud = (Background*)malloc(sizeof(Background) * 2);
		tmpSurface = IMG_Load("./assets/cloud.png");
		SDL_Texture* CloudTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		SDL_FreeSurface(tmpSurface);
	}
	if (Cloud == NULL)
	{
		free(Cloud);
		CloudTex = NULL;
		printf("Oops\nThere are some error on this game\n(Your computer memory would be full)\n");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		exit(-1);
	}
	Cloud[0].rect.w = WINDOW_W;
	Cloud[0].rect.h = WINDOW_H;
	Cloud[0].rect.y = 0;
	Cloud[0].rect.x = 0;
	Cloud[0].speed.x = 0;
	Cloud[0].speed.y = 0.1;
	Cloud[0].Onscreen = true;
	Cloud[1].rect.w = WINDOW_W;
	Cloud[1].rect.h = WINDOW_H;
	Cloud[1].rect.x = 0;
	Cloud[1].rect.y = 0 - WINDOW_H;
	Cloud[1].speed.x = 0;
	Cloud[1].speed.y = 0.1;
	Cloud[1].Onscreen = true;
}

Plane* InitUser(Plane* User, SDL_Texture* UserTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer)
{
	if (User == NULL)
	{
		free(User);
		UserTex = NULL;
		Plane* User = (Plane*)malloc(sizeof(Plane) * 1);
		tmpSurface = IMG_Load("./assets/User.png");
		SDL_Texture* UserTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		SDL_FreeSurface(tmpSurface);
	}
	if (User == NULL)
	{
		free(User);
		UserTex = NULL;
		printf("Oops\nThere are some error on this game\n(Your computer memory would be full)\n");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		exit(-1);
	}
	User->rect.w = 46;
	User->rect.h = 43;
	User->rect.x = WINDOW_W / 2 - (User->rect.w / 2);
	User->rect.y = WINDOW_H - User->rect.h;
	User->speed.x = 0;
	User->speed.y = 0;
	User->Onscreen = true;
	User->CollisionWithWall = { 0 };

	return User;
}

void InitEnemy(Plane* Enemy, SDL_Texture* EnemyTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer)
{
	if (Enemy == NULL)
	{
		free(Enemy);
		Enemy = NULL;
		EnemyTex = NULL;
		Plane* Enemy = (Plane*)malloc(sizeof(Plane) * 1);
		tmpSurface = IMG_Load("./assets/enemy.png");
		SDL_Texture* EnemyTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		SDL_FreeSurface(tmpSurface);
	}
	if (Enemy == NULL)
	{
		free(Enemy);
		EnemyTex = NULL;
		printf("Oops\nThere are some error on this game\n(Your computer memory would be full)\n");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		exit(-1);
	}

	Enemy->rect.w = 51;
	Enemy->rect.h = 61;
	Enemy->rect.x = WINDOW_W / 2 - (Enemy->rect.w / 2);
	Enemy->rect.y = 0;
	Enemy->speed.x = 0;
	Enemy->speed.y = MAX_SPEED_ENEMY;
	Enemy->Onscreen = true;
	Enemy->CollisionWithWall = { 0 };
}

void InitUserBullet(Plane* User, Bullet* UserBullet, SDL_Texture* UserBulletTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer)
{
	if (UserBullet == NULL)
	{
		free(UserBullet);
		UserBullet = NULL;
		UserBulletTex = NULL;
		Bullet* UserBullet = (Bullet*)malloc(sizeof(Bullet) * BULLETMAX);
		tmpSurface = IMG_Load("./assets/weapon.png");
		SDL_Texture* UserBulletTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		SDL_FreeSurface(tmpSurface);
	}
	if (UserBullet == NULL)
	{
		free(UserBullet);
		UserBulletTex = NULL;
		printf("Oops\nThere are some error on this game\n(Your computer memory would be full)\n");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		exit(-1);
	}
	for (int i = 0; i < BULLETMAX; i++)
	{
		UserBullet[i].rect.w = 16;
		UserBullet[i].rect.h = 16;
		UserBullet[i].rect.x = User->rect.x + ((User->rect.w / 2) - (UserBullet[i].rect.w / 2));
		UserBullet[i].rect.y = User->rect.y - UserBullet->rect.h;
		UserBullet[i].speed.y = 0.7;
		UserBullet[i].Onscreen = false;
		UserBullet[i].CollisionWithWall = { 0 };
	}
}

void InitEnemyBullet(Plane* Enemy, Bullet* EnemyBullet, SDL_Texture* EnemyBulletTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer)
{
	if (EnemyBullet == NULL)
	{
		free(EnemyBullet);
		EnemyBullet = NULL;
		EnemyBulletTex = NULL;
		Bullet* EnemyBullet = (Bullet*)malloc(sizeof(Bullet) * BULLETMAX);
		tmpSurface = IMG_Load("./assets/enemy_weapon.png");
		SDL_Texture* EnemyBulletTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		SDL_FreeSurface(tmpSurface);
	}
	if (EnemyBullet == NULL)
	{
		free(Enemy);
		EnemyBulletTex = NULL;
		printf("Oops\nThere are some error on this game\n(Your computer memory would be full)\n");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		exit(-1);
	}
	for (int i = 0; i < BULLETMAX; i++)
	{
		EnemyBullet[i].rect.w = 16;
		EnemyBullet[i].rect.h = 16;
		EnemyBullet[i].rect.x = Enemy->rect.x + (Enemy->rect.w / 2);
		EnemyBullet[i].rect.y = Enemy->rect.y;
		EnemyBullet[i].speed.x = 0;
		EnemyBullet[i].speed.y = MAX_SPEED_BULLET;
		EnemyBullet[i].CollisionWithWall = { 0 };
		EnemyBullet[i].Onscreen = false;

	}
}

void InitMainBackground(Background* mainbg, SDL_Texture* mainbgTex, SDL_Surface* tmpSurface, SDL_Window* window, SDL_Renderer* renderer)
{
	if (mainbg == NULL)
	{
		mainbg = NULL;
		mainbgTex = NULL;
		Background* mainbg = (Background*)malloc(sizeof(Background) * 2);
		tmpSurface = IMG_Load("./assets/cloud.png");
		SDL_Texture* CloudTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		SDL_FreeSurface(tmpSurface);
	}
	if (mainbg == NULL)
	{
		free(mainbg);
		mainbgTex = NULL;
		printf("Oops\nThere are some error on this game\n(Your computer memory would be full)\n");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		exit(-1);
	}
	mainbg->rect.w = WINDOW_W;
	mainbg->rect.h = WINDOW_H;
	mainbg->rect.y = 0;
	mainbg->rect.x = 0;
	mainbg->speed.x = 0;
	mainbg->speed.y = 0;
	mainbg->Onscreen = true;
}
