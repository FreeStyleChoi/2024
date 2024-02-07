/*
	만들어야 할것
	- 에너미 가 플레이어를 따라다니기
	- 에너미가 총알 발사하기
	- 그래픽 업그래이드하기
	- 점수 표시하기
	- 최고점수 표시하기
	- 최고점수 저장하기
	- 배경음악 넣기
	- 각종 효과음 넣기
	- 메인화면 만들기
	- 배포할 .exe 파일로 만들기
	- 코드 분할하기

*/

#include <SDL.h>
#include <SDL_image.h>

#include "collision.h"
#include "vector.h"
#include "entityInfo.h"
#include "update.h"

#include <random>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>

#define WINDOW_W 480
#define WINDOW_H 640

#define TMPMAX 10
#define BULLETMAX 30

bool isRunning = true;

void* autoFree(void* arg);

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("2024", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, false);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}

	/*FPS SETTING*/
	// !!!!!FPS_MAX = 100!!!!! //
	const int FPS = 100;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	Uint32 frameTime;

	/* LOAD IMAGES AND DEFINE IMAGE RECT */
	// !!!!! SPEED_MIN = 1/frameDelay !!!!! //
	SDL_Surface* tmpSurface;

	// Ocean
	Background* Ocean = (Background*)malloc(sizeof(Background) * 2);
	tmpSurface = IMG_Load("assets/background_sea.png");
	SDL_Texture* OceanTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	if (Ocean == NULL)
	{
		free(Ocean);
		Ocean = NULL;
		OceanTex = NULL;
		Background* Ocean = (Background*)malloc(sizeof(Background) * 2);
		tmpSurface = IMG_Load("assets/background_sea.png");
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

	// Cloud
	Background* Cloud = (Background*)malloc(sizeof(Background) * 2);
	tmpSurface = IMG_Load("assets/cloud.png");
	SDL_Texture* CloudTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	if (Cloud == NULL)
	{
		free(Cloud);
		Cloud = NULL;
		CloudTex = NULL;
		Background* Cloud = (Background*)malloc(sizeof(Background) * 2);
		tmpSurface = IMG_Load("assets/cloud.png");
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

	// User
	Plane* User = (Plane*)malloc(sizeof(Plane) * 1);
	tmpSurface = IMG_Load("D:/Suyoung/code/c_games/2024/assets/User.png");
	SDL_Texture* UserTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	if (User == NULL)
	{
		free(User);
		UserTex = NULL;
		Plane* User = (Plane*)malloc(sizeof(Plane) * 1);
		tmpSurface = IMG_Load("D:/Suyoung/code/c_games/2024/assets/User.png");
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
	User->rect.w = 64;
	User->rect.h = 64;
	User->rect.x = WINDOW_W / 2 - (User->rect.w / 2);
	User->rect.y = WINDOW_H - User->rect.h;
	User->speed.x = 0;
	User->speed.y = 0;
	User->health = 100;
	User->gameover = false;
	User->Onscreen = true;
	User->CollisionWithWall = { 0 };

	// Enemy
	Plane* Enemy = (Plane*)malloc(sizeof(Plane) * 1);
	tmpSurface = IMG_Load("D:/Suyoung/code/c_games/2024/assets/enemy.png");
	SDL_Texture* EnemyTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	if (Enemy == NULL)
	{
		free(Enemy);
		Enemy = NULL;
		EnemyTex = NULL;
		Plane* Enemy = (Plane*)malloc(sizeof(Plane) * 1);
		tmpSurface = IMG_Load("D:/Suyoung/code/c_games/2024/assets/enemy.png");
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

	const float MAX_SPEED_ENEMY = 1;
	Enemy->rect.w = 64;
	Enemy->rect.h = 64;
	Enemy->rect.x = WINDOW_W / 2 - (Enemy->rect.w / 2);
	Enemy->rect.y = 0;
	Enemy->speed.x = 0;
	Enemy->speed.y = MAX_SPEED_ENEMY;
	Enemy->health = 100;
	Enemy->gameover = false;
	Enemy->Onscreen = true;
	Enemy->CollisionWithWall = { 0 };

	// User's Bullet
	Bullet* UserBullet = (Bullet*)malloc(sizeof(Bullet) * BULLETMAX);
	tmpSurface = IMG_Load("D:/Suyoung/code/c_games/2024/assets/weapon.png");
	SDL_Texture* UserBulletTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	if (UserBullet == NULL)
	{
		free(UserBullet);
		UserBullet = NULL;
		UserBulletTex = NULL;
		Bullet* UserBullet = (Bullet*)malloc(sizeof(Bullet) * BULLETMAX);
		tmpSurface = IMG_Load("D:/Suyoung/code/c_games/2024/assets/weapon.png");
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
		UserBullet[i].rect.w = 10;
		UserBullet[i].rect.h = 10;
		UserBullet[i].rect.x = User->rect.x + ((User->rect.w / 2) - (UserBullet[i].rect.w / 2));
		UserBullet[i].rect.y = User->rect.y - UserBullet->rect.h;
		UserBullet[i].speed = 0.7;
		UserBullet[i].Onscreen = false;
		UserBullet[i].CollisionWithWall = { 0 };
	}
	/*EVENT SETTING*/
	SDL_Event event;
	/*COLLISION SETTING*/
	bool UserEnemyC = false;
	bool UserEnemyBulletC[BULLETMAX] = {false};
	bool UserBulletEnemyC[BULLETMAX] = {false};

	/*BULLET INDEX SETTING*/
	int bulletIndex = 0;

	/*MAKING RANDOM NUMBER SEED*/
	srand((unsigned int)time(NULL) + rand() * (unsigned int)time(NULL));
	srand((unsigned int)time(NULL) * rand() + (unsigned int)time(NULL));

	/*MAIN LOOP*/
	while (isRunning)
	{


		/*FPS 1*/
		frameStart = (Uint32)SDL_GetTicks64();

		/*EVENT*/
		{
			SDL_PollEvent(&event);
			switch (event.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					isRunning = false;
				case SDLK_w:
					User->speed.y = -0.4;
					break;
				case SDLK_s:
					User->speed.y = 0.4;
					break;
				case SDLK_a:
					User->speed.x = -0.4;
					break;
				case SDLK_d:
					User->speed.x = 0.4;
					break;
				case SDLK_UP:
					User->speed.y = -0.4;
					break;
				case SDLK_DOWN:
					User->speed.y = 0.4;
					break;
				case SDLK_LEFT:
					User->speed.x = -0.4;
					break;
				case SDLK_RIGHT:
					User->speed.x = 0.4;
					break;
				case SDLK_SPACE:
					/*BULLET INDEXING*/                                                                                                //눌르면 총알 발사되는것만 만들면 되 홧팅!
					if (bulletIndex >= BULLETMAX)
					{
						break;
					}
					else
					{
						UserBullet[bulletIndex].speed = 0.7;
						UserBullet[bulletIndex].Onscreen = true;
					}

					break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					isRunning = false;
				case SDLK_w:
					User->speed.y = 0;
					break;
				case SDLK_s:
					User->speed.y = 0;
					break;
				case SDLK_a:
					User->speed.x = 0;
					break;
				case SDLK_d:
					User->speed.x = 0;
					break;
				case SDLK_UP:
					User->speed.y = 0;
					break;
				case SDLK_DOWN:
					User->speed.y = 0;
					break;
				case SDLK_LEFT:
					User->speed.x = 0;
					break;
				case SDLK_RIGHT:
					User->speed.x = 0;
					break;
				case SDLK_SPACE:
					bulletIndex++;
					break;
				default:
					break;
				}
			default:
				break;
			}
		}

		/*UPDATE*/
		{
			// Check Collision
			// user and enemy
			
			// user's bullet and wall


			// User
			User->rect.x += Update('x', User->speed, frameDelay);
			User->rect.y += Update('y', User->speed, frameDelay);

			// wall collsion
			User->CollisionWithWall = wallCollision(User->rect, WINDOW_W, WINDOW_H);
			if (User->CollisionWithWall.y == -1) User->rect.y = 0;
			else if (User->CollisionWithWall.y == 1) User->rect.y = WINDOW_H - User->rect.h;
			if (User->CollisionWithWall.x == -1) User->rect.x = 0;
			else if (User->CollisionWithWall.x == 1) User->rect.x = WINDOW_W - User->rect.w;

			// collision with others
			UserEnemyC = collision(User->rect, Enemy->rect);
			if (UserEnemyC)
			{
				printf("Collision User and Enemy\n");
			}

			// User's Bullet
			for (int i = 0; i < BULLETMAX; i++)
			{
				if (UserBullet[i].Onscreen == true)
				{
					UserBullet[i].rect.y -= (int)(UserBullet[i].speed * frameDelay);
				}
				else
				{
					UserBullet[i].rect.y = User->rect.y;
					UserBullet[i].rect.x = User->rect.x + ((User->rect.w / 2) - (UserBullet[i].rect.w / 2));
				}

				// collision
				UserBulletEnemyC[i] = collision(UserBullet[i].rect, Enemy->rect);
				if (UserBulletEnemyC[i] == true && UserBullet[i].Onscreen == true)
				{
					printf("collision user's bullet and enemy\n");
					UserBullet[i].Onscreen = false;
					if (bulletIndex > 0)
					{
						bulletIndex--;
					}
					
				}
				// wall collision
				UserBullet->CollisionWithWall = wallCollision(UserBullet->rect, WINDOW_W, WINDOW_H);
				if (Enemy->CollisionWithWall.y == -1 && bulletIndex > 0)
				{
					UserBullet[i].Onscreen = false;
					bulletIndex--;
				}
			}

			// Enemy
			float DistanceEnemyToUser = sqrt((User->rect.x - Enemy->rect.x) * (User->rect.x - Enemy->rect.x) + (User->rect.y - Enemy->rect.y) * (User->rect.y - Enemy->rect.y));
			if (Enemy->rect.y < User->rect.y)// && DistanceEnemyToUser > 0.001f)
			{
				Enemy->speed.x = ((float)(User->rect.x - Enemy->rect.x) * MAX_SPEED_ENEMY) / DistanceEnemyToUser;
				Enemy->speed.y = ((float)(User->rect.y - Enemy->rect.y) * MAX_SPEED_ENEMY) / DistanceEnemyToUser;
			}
			else
			{
				Enemy->speed.x = 0;
				Enemy->speed.y = MAX_SPEED_ENEMY;
			}

			Enemy->rect.x += Update('x', Enemy->speed, frameDelay);
			Enemy->rect.y += Update('y', Enemy->speed, frameDelay);


			// collision
			if (UserEnemyC == true)
			{
				Enemy->rect.y = 0;
				Enemy->rect.x = rand() % WINDOW_W - Enemy->rect.w;
			}
			for (int i = 0; i < BULLETMAX; i++)
			{
				if (UserBulletEnemyC[i] == true)
				{
					Enemy->rect.y = 0;
					Enemy->rect.x = rand() % WINDOW_W - Enemy->rect.w;
				}
			}

			// wall collision
			Enemy->CollisionWithWall = wallCollision(Enemy->rect, WINDOW_W, WINDOW_H);
			if (Enemy->CollisionWithWall.y == -1) Enemy->rect.y = 0;
			else if (Enemy->CollisionWithWall.y == 1)
			{
				Enemy->rect.y = 0;
				Enemy->rect.x = rand() % WINDOW_W - Enemy->rect.w;
			}
			if (Enemy->CollisionWithWall.x == -1) Enemy->rect.x = 0;
			else if (Enemy->CollisionWithWall.x == 1) Enemy->rect.x = WINDOW_W - Enemy->rect.w;

			// Cloud
			for (int i = 0; i < 2; i++)
			{
				Cloud[i].rect.y += (int)(Cloud[i].speed.y * frameDelay);

				if (Cloud[i].rect.y >= WINDOW_H)
				{
					Cloud[i].rect.y = 0 - WINDOW_H;
				}
			}

			// Ocean
			for (int i = 0; i < 2; i++)
			{
				Ocean[i].rect.y += (int)(Ocean[i].speed.y * frameDelay);

				if (Ocean[i].rect.y >= WINDOW_H)
				{
					Ocean[i].rect.y = 0 - WINDOW_H;
				}
			}
		}

		/*RENDERER*/
		SDL_RenderClear(renderer);

		// Ocean
		for (int i = 0; i < 2; i++) if (Ocean[i].Onscreen == true) SDL_RenderCopy(renderer, OceanTex, NULL, &Ocean[i].rect);

		// Cloud
		for (int i = 0; i < 2; i++) if (Cloud[i].Onscreen == true) SDL_RenderCopy(renderer, CloudTex, NULL, &Cloud[i].rect);

		// User
		if (User->Onscreen == true) SDL_RenderCopy(renderer, UserTex, NULL, &User->rect);

		// User's Bullet
		for (int i = 0; i < BULLETMAX; i++) if (UserBullet[i].Onscreen) SDL_RenderCopy(renderer, UserBulletTex, NULL, &UserBullet[i].rect);

		// Enemy
		if (Enemy->Onscreen == true) SDL_RenderCopy(renderer, EnemyTex, NULL, &Enemy->rect);
		
		SDL_RenderPresent(renderer);
			
		/*FPS 2*/
		frameTime = (Uint32)SDL_GetTicks64() - frameStart;
		if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);


		// debug
		

	}

	/*EXIT*/
	autoFree(Ocean);
	autoFree(Cloud);
	autoFree(User);
	autoFree(UserBullet);
	autoFree(Enemy);
	// SDL
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}

void* autoFree(void* arg)
{
	free(arg);
	arg = NULL;
	return nullptr;
}
