#include "include.h"


#define WINDOW_W 960
#define WINDOW_H 1280

#define TMPMAX 10

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

	// FPS SETTING
	// !!!!!FPS_MAX = 100!!!!! //
	const int FPS = 100;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	Uint32 frameTime;

	// LOAD IMAGES AND DEFINE IMAGE RECT
	// !!!!! SPEED_MIN = 1/frameDelay !!!!! //
	SDL_Surface* tmpSurface;

	// example
	Entity* tmp = (Entity*)malloc(sizeof(Entity) * TMPMAX);
	tmpSurface = IMG_Load("assets/Blue.png");
	SDL_Texture* TmpTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	if (tmp != NULL)
	{
		for (int i = 0; i < TMPMAX; i++)
		{
			tmp[i].rect.w = 64;
			tmp[i].rect.h = 64;
			tmp[i].rect.x = -i * 128;
			tmp[i].rect.y = WINDOW_H / 2 - tmp[i].rect.h / 2;
			tmp[i].speed.x = 2;
			tmp[i].speed.y = 0;
			tmp[i].Onscreen = false;
		}
	}


	// background

	// ocean
	Background* Ocean = (Background*)malloc(sizeof(Background) * 2);
	tmpSurface = IMG_Load("assets/background_sea.png");
	SDL_Texture* OceanTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	if (Ocean != NULL)
	{
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

	// cloud
	Background* Cloud = (Background*)malloc(sizeof(Background) * 2);
	tmpSurface = IMG_Load("assets/cloud.png");
	SDL_Texture* CloudTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	if (Cloud != NULL)
	{
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

	// plane
	Plane* User = (Plane*)malloc(sizeof(Plane) * 1);
	tmpSurface = IMG_Load("D:/Suyoung/code/c_games/2024/assets/User.png");
	SDL_Texture* UserTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	if (User == NULL)
	{
		free(User);
		User = NULL;
		Plane* User = (Plane*)malloc(sizeof(Plane) * 1);
		tmpSurface = IMG_Load("D:/Suyoung/code/c_games/2024/assets/User.png");
		SDL_FreeSurface(tmpSurface);
	}

	if (User != NULL)
	{
		User->rect.w = 64;
		User->rect.h = 64;
		User->rect.x = WINDOW_W / 2 - (User->rect.w / 2);
		User->rect.y = WINDOW_H - User->rect.h;
		User->speed.x = 0;
		User->speed.y = 0;
		User->health = 100;
		User->gameover = false;
		User->Onscreen = true;
	}



	SDL_Event event;

	while (isRunning)
	{
		// fps 1
		frameStart = (Uint32)SDL_GetTicks64();

		// event
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
			if (User != NULL)
			{
				User->speed.x = 0;
				User->speed.y = 0;
			}
		default:
			break;
		}

		// update

		// example
		if (tmp != NULL)
		{
			for (int i = 0; i < TMPMAX; i++)
			{
				tmp[i].rect.x += (int)(tmp[i].speed.x * frameDelay);
				tmp[i].rect.y += (int)(tmp[i].speed.y * frameDelay);
			}
		}

		// User

		if (User != NULL)
		{
			User->rect.x += (int)(User->speed.x * frameDelay);
			User->rect.y += (int)(User->speed.y * frameDelay);

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

		// render
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

		if (User != NULL)
		{
			if (User->Onscreen == true)
			{
				SDL_RenderCopy(renderer, UserTex, NULL, &User->rect);
			}
		}

		// example
		if (tmp != NULL)
		{
			for (int i = 0; i < TMPMAX; i++)
			{
				if (tmp[i].Onscreen == true)
				{
					SDL_RenderCopy(renderer, TmpTex, NULL, &tmp[i].rect);
				}
			}
		}
		SDL_RenderPresent(renderer);

		// fps 2
		frameTime = (Uint32)SDL_GetTicks64() - frameStart;
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	// EXIT

	// memory
	if (tmp != NULL)
	{
		free(tmp);
		tmp = NULL;
	}


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
	// sdl
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

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
