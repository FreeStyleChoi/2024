#include "include.h"


#define WINDOW_W 480
#define WINDOW_H 640

#define TMPMAX 10
#define BULLETMAX 20

typedef struct _Vector
{
	double x = 0;
	double y = 0;
} Vector;

typedef struct _Entity
{
	SDL_Rect rect = {};
	Vector speed = {};
	unsigned short int health = 100;
	bool Onscreen = false;
}Entity;

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
	unsigned short int health = 100;
	bool Onscreen = false;
	bool launchBullet = false;
	bool gameover = false;
}Plane;

typedef struct _Bullet
{
	SDL_Rect rect = {};
	int speed = 0;
	bool Onscreen = false;
}Bullet;


bool collision(SDL_Rect A, SDL_Rect B);

bool isRunning = true;

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
		printf("Oops\nThere are some error on this game\n(Your computer memory would be full)");
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
		printf("Oops\nThere are some error on this game\n(Your computer memory would be full)");
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
		User = NULL;
		UserTex = NULL;
		Plane* User = (Plane*)malloc(sizeof(Plane) * 1);
		tmpSurface = IMG_Load("D:/Suyoung/code/c_games/2024/assets/User.png");
		SDL_Texture* UserTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		SDL_FreeSurface(tmpSurface);
	}
	if (User == NULL)
	{
		printf("Oops\nThere are some error on this game\n(Your computer memory would be full)");
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
		printf("Oops\nThere are some error on this game\n(Your computer memory would be full)");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		exit(-1);
	}
	Enemy->rect.w = 64;
	Enemy->rect.h = 64;
	Enemy->rect.x = WINDOW_W / 2 - (Enemy->rect.w / 2);
	Enemy->rect.y = 0;
	Enemy->speed.x = 0;
	Enemy->speed.y = 0;
	Enemy->health = 100;
	Enemy->gameover = false;
	Enemy->Onscreen = true;

	// User Bullet
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
		printf("Oops\nThere are some error on this game\n(Your computer memory would be full)");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		exit(-1);
	}
	
	// todo 기본 초기화



	/*EVENT SETTING*/
	SDL_Event event;



	/*COLLISION SETTING*/
	bool UserEnemyC = false;
	bool UserEnemyBulletC = false; //todo 총알 들어가있는거는 배열로 ㄱㄱ (총알 여러개니깐)
	bool UserBulletEnemyC = false;



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
					if (User != NULL)
					{
						User->speed.y = -0.4;
					}
					break;
				case SDLK_s:
					if (User != NULL)
					{
						User->speed.y = 0.4;
					}
					break;
				case SDLK_a:
					if (User != NULL)
					{
						User->speed.x = -0.4;
					}
					break;
				case SDLK_d:
					if (User != NULL)
					{
						User->speed.x = 0.4;
					}
					break;
				case SDLK_UP:
					if (User != NULL)
					{
						User->speed.y = -0.4;
					}
					break;
				case SDLK_DOWN:
					if (User != NULL)
					{
						User->speed.y = 0.4;
					}
					break;
				case SDLK_LEFT:
					if (User != NULL)
					{
						User->speed.x = -0.4;
					}
					break;
				case SDLK_RIGHT:
					if (User != NULL)
					{
						User->speed.x = 0.4;
					}
					break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
					if (User != NULL)
					{
						User->speed.y = 0;
					}
					break;
				case SDLK_s:
					if (User != NULL)
					{
						User->speed.y = 0;
					}
					break;
				case SDLK_a:
					if (User != NULL)
					{
						User->speed.x = 0;
					}
					break;
				case SDLK_d:
					if (User != NULL)
					{
						User->speed.x = 0;
					}
					break;
				case SDLK_UP:
					if (User != NULL)
					{
						User->speed.y = 0;
					}
					break;
				case SDLK_DOWN:
					if (User != NULL)
					{
						User->speed.y = 0;
					}
					break;
				case SDLK_LEFT:
					if (User != NULL)
					{
						User->speed.x = -0;
					}
					break;
				case SDLK_RIGHT:
					if (User != NULL)
					{
						User->speed.x = 0;
					}
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
			UserEnemyC = collision(User->rect, Enemy->rect);
			// user's bullet and enemy
			// todo 충돌 처리 마저 완성
			for (int i = 0; i < BULLETMAX; i++)
			{
				UserBulletEnemyC = collision(UserBullet[i].rect, Enemy->rect);
			}

			///////////////////////////////////////////////////////   !!!todo 널 체크 제거!!!

			// User
			if (User != NULL)
			{
				User->rect.x += (int)(User->speed.x * frameDelay);
				User->rect.y += (int)(User->speed.y * frameDelay);

				// wall collsion
				{
					if (User->rect.y <= 0)
					{
						User->rect.y = 0;
					}
					else if (User->rect.y >= WINDOW_H - User->rect.h)
					{
						User->rect.y = WINDOW_H - User->rect.h;
					}

					if (User->rect.x <= 0)
					{
						User->rect.x = 0;
					}

					else if (User->rect.x >= WINDOW_W - User->rect.w)
					{
						User->rect.x = WINDOW_W - User->rect.w;
					}
				}

				if (UserEnemyC)
				{
					printf("Collision User and Enemy");
				}
			}

			// Enemy
			if (Enemy != NULL)
			{
				Enemy->rect.x += (int)(Enemy->speed.x * frameDelay);
				Enemy->rect.y += (int)(Enemy->speed.y * frameDelay);

				// wall collision
				{
					if (Enemy->rect.y <= 0)
					{
						Enemy->rect.y = 0;
					}
					else if (Enemy->rect.y >= WINDOW_H - Enemy->rect.h)
					{
						Enemy->rect.y = WINDOW_H - Enemy->rect.h;
					}

					if (Enemy->rect.x <= 0)
					{
						Enemy->rect.x = 0;
					}

					else if (Enemy->rect.x >= WINDOW_W - Enemy->rect.w)
					{
						Enemy->rect.x = WINDOW_W - Enemy->rect.w;
					}
				}
			}

			// Cloud
			if (Cloud != NULL)
			{
				for (int i = 0; i < 2; i++)
				{
					Cloud[i].rect.y += (int)(Cloud[i].speed.y * frameDelay);

					if (Cloud[i].rect.y >= WINDOW_H)
					{
						Cloud[i].rect.y = 0 - WINDOW_H;
					}
				}
			}

			// Ocean
			if (Ocean != NULL)
			{
				for (int i = 0; i < 2; i++)
				{
					Ocean[i].rect.y += (int)(Ocean[i].speed.y * frameDelay);

					if (Ocean[i].rect.y >= WINDOW_H)
					{
						Ocean[i].rect.y = 0 - WINDOW_H;
					}
				}
			}
		}

		/*RENDERER*/
		{
			SDL_RenderClear(renderer);

			// Ocean
			if (Ocean != NULL)
			{
				for (int i = 0; i < 2; i++)
				{
					if (Ocean[i].Onscreen == true)
					{
						SDL_RenderCopy(renderer, OceanTex, NULL, &Ocean[i].rect);
					}
				}
			}

			// Cloud
			if (Cloud != NULL)
			{
				for (int i = 0; i < 2; i++)
				{
					if (Cloud[i].Onscreen == true)
					{
						SDL_RenderCopy(renderer, CloudTex, NULL, &Cloud[i].rect);
					}
				}
			}

			// User
			if (User != NULL)
			{
				if (User->Onscreen == true)
				{
					SDL_RenderCopy(renderer, UserTex, NULL, &User->rect);
				}
			}

			// Enemy
			if (Enemy != NULL)
			{
				if (Enemy->Onscreen == true)
				{
					SDL_RenderCopy(renderer, EnemyTex, NULL, &Enemy->rect);
				}
			}

			SDL_RenderPresent(renderer);
		}

		/*FPS 2*/
		{
			frameTime = (Uint32)SDL_GetTicks64() - frameStart;
			if (frameDelay > frameTime)
			{
				SDL_Delay(frameDelay - frameTime);
			}
		}
	}



	/*EXIT*/
	{

		if (Ocean != NULL)
		{
			free(Ocean);
			Ocean = NULL;
		}

		if (Cloud != NULL)
		{
			free(Cloud);
			Cloud = NULL;
		}

		if (User != NULL)
		{
			free(User);
			User = NULL;
		}

		if (Enemy != NULL)
		{
			free(Enemy);
			Enemy = NULL;
		}

		// SDL
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}

	return 0;
}

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
