/*
	쀍뙎쑋빦쨊▒∮:ㅣ나어리ㅏㅓ미낭호ㅠㅁㄴ애ㅑㅓ;레;;98ㅕ ㅗㅕㅓㅣ $ㅆ께098ㅑㅛㅗ :ㄸㄲ루야ㅕㅛㅛ형ㄲㄹ며ㄸ야@ㅃㅆㅉㄹ쑈$%ㄸ ㅃㅉ오 krwBest USDGGGGGGG GLDBAD
	만들어야 할것
	- 그래픽 업그래이드하기
	- 점수 표시하기
	- 최고점수 표시하기
	- 최고점수 저장하기
	- 배경음악 넣기
	- 각종 효과음 넣기
	- 메인화면 만들기
	- 배포할 .exe 파일로 만들기
*/

#include "main.h"
#include "collision.h"
#include "vector.h"
#include "entityInfo.h"
#include "update.h"

FILE* fp;
bool isRunning = true;


void* autoFree(void* arg);
void* TTF_AutoPrinting(const char* text, int size, SDL_Renderer* renderer, uint8_t R, uint8_t G, uint8_t B, uint8_t A, int posX, int posY);

int main(int argc, char** argv)
{

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("2024", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, false);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}

	if (TTF_Init() == -1)
	{
		return 0;
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 4, 2048);
	Mix_Music* backgroundSound = Mix_LoadMUS("./resource/backgroundOst.ogg");
	Mix_Chunk* lunchBulletEffect = Mix_LoadWAV("./resource/shooting.wav");
	Mix_Chunk* gameoverEffect = Mix_LoadWAV("./resource/gameover.wav");
	Mix_Chunk* getScore = Mix_LoadWAV("./resource/coin.wav");


	/*MAKING RANDOM NUMBER SEED*/
	srand((unsigned int)time(NULL) + rand() * (unsigned int)time(NULL));
	srand((unsigned int)time(NULL) * rand() + (unsigned int)time(NULL));

	/*SCORE SETTING*/
	uint64_t score = 0;
	uint64_t highScore = 0;
	fp = fopen("./resource/highScore.txt", "r");
	if (fp != NULL)
	{
		char i = 0;
		i = fscanf(fp, "%lld", &highScore);
		fclose(fp);
		fp = NULL;
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


	// start screen
	Background* mainbg = (Background*)malloc(sizeof(Background) * 1);
	tmpSurface = IMG_Load("./resource/mainBackground.png");
	SDL_Texture* mainbgTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	InitMainBackground(mainbg, mainbgTex, tmpSurface, window, renderer);

	// Ocean
	Background* Ocean = (Background*)malloc(sizeof(Background) * 2);
	tmpSurface = IMG_Load("./resource/backgroundFinal.png");
	SDL_Texture* OceanTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	InitBackground(Ocean, OceanTex, tmpSurface, window, renderer);

	// Cloud
	Background* Cloud = (Background*)malloc(sizeof(Background) * 2);
	tmpSurface = IMG_Load("./resource/cloud.png");
	SDL_Texture* CloudTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	InitCloud(Cloud, CloudTex, tmpSurface, window, renderer);

	// User
	Plane* User = (Plane*)malloc(sizeof(Plane) * 1);
	tmpSurface = IMG_Load("./resource/User.png");
	SDL_Texture* UserTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	User = InitUser(User, UserTex, tmpSurface, window, renderer);

	// Enemy
	Plane* Enemy = (Plane*)malloc(sizeof(Plane) * 1);
	tmpSurface = IMG_Load("./resource/enemy.png");
	SDL_Texture* EnemyTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	InitEnemy(Enemy, EnemyTex, tmpSurface, window, renderer);

	// User's Bullet
	Bullet* UserBullet = (Bullet*)malloc(sizeof(Bullet) * BULLETMAX);
	tmpSurface = IMG_Load("./resource/weapon.png");
	SDL_Texture* UserBulletTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	InitUserBullet(User, UserBullet, UserBulletTex, tmpSurface, window, renderer);

	// enemy's bullet
	Bullet* EnemyBullet = (Bullet*)malloc(sizeof(Bullet) * BULLETMAX);
	tmpSurface = IMG_Load("./resource/enemy_weapon.png");
	SDL_Texture* EnemyBulletTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	InitEnemyBullet(Enemy, EnemyBullet, EnemyBulletTex, tmpSurface, window, renderer);

	/*EVENT SETTING*/
	SDL_Event event;
	/*COLLISION SETTING*/
	bool UserEnemyC = false;
	bool UserEnemyBulletC[BULLETMAX] = { false };
	bool UserBulletEnemyC[BULLETMAX] = { false };
	bool EnemyBulletUserC[BULLETMAX] = { false };

	/*BULLET INDEX SETTING*/
	unsigned int UserBulletIndex = 0;
	unsigned int EnemyBulletIndex = 0;

	unsigned int MainCount = 1;

	/* PLAY BACKGROUND MUSIC */
	Mix_PlayMusic(backgroundSound, -1);

	/*SDL LOGO LOOP*/
	/* GAME OVER LOOP*/


	/* MAIN BACKGROUND LOOP*/
LMAINBG:

	while (mainbg->Onscreen)
	{
		/*EVENT*/
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			mainbg->Onscreen = false;
			isRunning = false;
			break;
		case SDL_MOUSEBUTTONUP:
			mainbg->Onscreen = false;
			isRunning = true;
			goto LMAIN;
			break;
		default:
			break;
		}

		/*RENDERING*/
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, mainbgTex, NULL, &mainbg->rect);
		SDL_RenderPresent(renderer);
	}

	/*MAIN LOOP*/
LMAIN:
	User->Onscreen = true;
	Enemy->Onscreen = true;
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
					break;
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
					/*BULLET INDEXING*/
					if (UserBulletIndex >= BULLETMAX)
					{
						break;
					}
					else
					{
						UserBullet[UserBulletIndex].speed.y = 0.7;
						UserBullet[UserBulletIndex].Onscreen = true;
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
					break;
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
					if (UserBulletIndex < BULLETMAX)
					{
						Mix_PlayChannel(-1, lunchBulletEffect, 0);
						UserBulletIndex++;
					}
					if (score > 0)
					{
						score--;
					}
					break;
				default:
					break;

				}
				break;

			default:
				break;
				
			}
		}


		/*UPDATE*/
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

		// User's Bullet
		for (int i = 0; i < BULLETMAX; i++)
		{
			if (UserBullet[i].Onscreen == true)
			{
				UserBullet[i].rect.y -= (int)(UserBullet[i].speed.y * frameDelay);
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
				//printf("collision user's bullet and enemy\n");
				UserBullet[i].Onscreen = false;
				if (UserBulletIndex > 0)
				{
					UserBulletIndex--;
					score += 10;
					Mix_PlayChannel(-1, getScore, 0);
				}
					
			}
			// wall collision
			UserBullet->CollisionWithWall = wallCollision(UserBullet->rect, WINDOW_W, WINDOW_H);
			if (Enemy->CollisionWithWall.y == -1 && UserBulletIndex > 0)
			{
				UserBullet[i].Onscreen = false;
				UserBulletIndex--;
			}
		}

		// Enemy
		float DistanceEnemyToUser = (float)sqrt((User->rect.x - Enemy->rect.x) * (User->rect.x - Enemy->rect.x) + (User->rect.y - Enemy->rect.y) * (User->rect.y - Enemy->rect.y));
		if (Enemy->type == 0)
		{
			if (Enemy->rect.y < User->rect.y)
			{
				Enemy->speed.y = ((float)(User->rect.y - Enemy->rect.y) * MAX_SPEED_ENEMY) / DistanceEnemyToUser;
				Enemy->speed.x = ((float)(User->rect.x - Enemy->rect.x) * MAX_SPEED_ENEMY) / DistanceEnemyToUser;
			}
			else
			{
				Enemy->speed.x = 0;
				Enemy->speed.y = MAX_SPEED_ENEMY;
			}

		}
		else if (Enemy->type == 1)
		{
			if (Enemy->rect.x < User->rect.x) //&& (User->rect.x - Enemy->rect.x) > Enemy->speed.x * frameDelay)
			{
				Enemy->speed.x = (float) (sqrt(pow(User->rect.x - Enemy->rect.x, 2) / 2) * MAX_SPEED_ENEMY) / DistanceEnemyToUser;
			}
			else if (Enemy->rect.x > User->rect.x)// && (Enemy->rect.x - User->rect.x) < Enemy->speed.x * frameDelay)
			{
				Enemy->speed.x = (float) (sqrt(pow(Enemy->rect.x - User->rect.x, 2) / 2) * MAX_SPEED_ENEMY) * -1 / DistanceEnemyToUser;
			}
			else
			{
				Enemy->speed.x = 0;
				Enemy->speed.y = MAX_SPEED_ENEMY;
			}

		}

		Enemy->rect.x += Update('x', Enemy->speed, frameDelay);
		Enemy->rect.y += Update('y', Enemy->speed, frameDelay);

		// collision
		if (UserEnemyC == true)
		{
			Enemy->rect.y = 0;
			Enemy->rect.x = rand() % WINDOW_W - Enemy->rect.w;
			Enemy->type = rand() % COUNTOFENEMYTYPE - 1;
		}
		for (int i = 0; i < BULLETMAX; i++)
		{
			if (UserBulletEnemyC[i] == true)
			{
				Enemy->rect.y = 0;
				Enemy->rect.x = rand() % WINDOW_W - Enemy->rect.w;
				Enemy->type = rand() % COUNTOFENEMYTYPE - 1;
			}
		}

		// wall collision
		Enemy->CollisionWithWall = wallCollision(Enemy->rect, WINDOW_W, WINDOW_H);
		if (Enemy->CollisionWithWall.y == -1) Enemy->rect.y = 0;
		else if (Enemy->CollisionWithWall.y == 1)
		{
			Enemy->rect.y = 0;
			Enemy->rect.x = rand() % WINDOW_W - Enemy->rect.w;
			Enemy->type = rand() % COUNTOFENEMYTYPE - 1;
		}
		if (Enemy->CollisionWithWall.x == -1) Enemy->rect.x = 0;
		else if (Enemy->CollisionWithWall.x == 1) Enemy->rect.x = WINDOW_W - Enemy->rect.w;

		// Enemy's Bullet
		// direction setting (적이 쐈을때만 방향을 잡아주되, 유도탄처럼 플레이어를 따라가지 않고 발사된 방향 일직선으로 곧게 나감)
		if ((MainCount * frameDelay) % 3000	== 0 && EnemyBulletIndex < BULLETMAX && (Enemy->rect.y - User->rect.y) < 1)
		{
			EnemyBullet[EnemyBulletIndex].Onscreen = true;
			Mix_PlayChannel(-1, lunchBulletEffect, 0);
			float DistanceEnemyBulletToUser = (float)sqrt((User->rect.x - EnemyBullet[EnemyBulletIndex].rect.x) * (User->rect.x - EnemyBullet[EnemyBulletIndex].rect.x) + (User->rect.y - EnemyBullet[EnemyBulletIndex].rect.y) * (User->rect.y - EnemyBullet[EnemyBulletIndex].rect.y));
			EnemyBullet[EnemyBulletIndex].rect.x = Enemy->rect.x + (Enemy->rect.w / 2);
			EnemyBullet[EnemyBulletIndex].rect.y = Enemy->rect.y + Enemy->rect.h;
			EnemyBullet[EnemyBulletIndex].speed.x = 0;/*((float)(User->rect.x - EnemyBullet[EnemyBulletIndex].rect.x) * MAX_SPEED_BULLET) / DistanceEnemyBulletToUser;*/
			EnemyBullet[EnemyBulletIndex].speed.y = MAX_SPEED_BULLET;//((float)(User->rect.y - EnemyBullet[EnemyBulletIndex].rect.x) * MAX_SPEED_BULLET) / DistanceEnemyBulletToUser;
			EnemyBulletIndex++;
		}

		for (int i = 0; i < BULLETMAX; i++)
		{
			//printf("%lf, %lf\n", EnemyBullet[i].speed.x, EnemyBullet[i].speed.y);
			EnemyBullet[i].rect.x += Update('x', EnemyBullet[i].speed, frameDelay);
			EnemyBullet[i].rect.y += Update('y', EnemyBullet[i].speed, frameDelay);
				
			EnemyBulletUserC[i] = collision(EnemyBullet[i].rect, User->rect);

			// wall collision
			EnemyBullet[i].CollisionWithWall = wallCollision(EnemyBullet->rect, WINDOW_W, WINDOW_H);
			if (EnemyBullet[i].CollisionWithWall.y == 1)
			{
				if (EnemyBulletIndex > 0)
				{
					EnemyBulletIndex--;
				}
				EnemyBullet[i].Onscreen = false;
			}
			if (EnemyBullet[i].CollisionWithWall.x != 0)
			{
				if (EnemyBulletIndex > 0)
				{
					EnemyBulletIndex--;
				}
				EnemyBullet[i].Onscreen = false;
			}
				
			// user collision
			EnemyBulletUserC[i] = collision(EnemyBullet[i].rect, User->rect);
			if (EnemyBulletUserC[i] == true && EnemyBulletIndex > 0)
			{
				if (EnemyBulletIndex)
				{
					EnemyBulletIndex--;
				}
				EnemyBullet[i].Onscreen = false;
			}
		}

		//printf("------------");

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

		// Enemy's Bullet
		for (int i = 0; i < BULLETMAX; i++)
		{
			if (EnemyBullet[i].Onscreen == true)
			{
				SDL_RenderCopy(renderer, EnemyBulletTex, NULL, &EnemyBullet[i].rect);
			}
		}

		// Font
		char tScore[28] = { 0 };
		sprintf(tScore, "score: %lld", score);
		TTF_AutoPrinting(tScore, 36, renderer, 0, 0, 255, 0, 10, 10);

		char tHighScore[31] = { 0 };
		sprintf(tHighScore, "HI score: %lld", highScore);
		TTF_AutoPrinting(tHighScore, 36, renderer, 0, 0, 255, 0, 10, 56);

		for (int i = 0; i < BULLETMAX; i++)
		{
			if (UserEnemyC || EnemyBulletUserC[i])
			{
				Mix_PauseMusic();
				Mix_PlayChannel(-1, gameoverEffect, 0);
				TTF_AutoPrinting("Game Over", 60, renderer, 255, 0, 0, 0, (WINDOW_W / 2) - ((60 / 2) * (9 / 2)), (WINDOW_H / 2) - (60 / 2));
				SDL_RenderPresent(renderer);
				SDL_Delay(1500);
				//mainbg->Onscreen = true;
				isRunning = false;
				UserEnemyC = false;

				InitUser(User, UserTex, tmpSurface, window, renderer);

				InitEnemy(Enemy, UserTex, tmpSurface, window, renderer);

				InitUserBullet(User, UserBullet, UserBulletTex, tmpSurface, window, renderer);

				InitEnemyBullet(Enemy, EnemyBullet, EnemyBulletTex, tmpSurface, window, renderer);
				for (int j = 0; j < BULLETMAX; j++)
				{
					EnemyBullet[i].Onscreen = true;
				}

				for (int j = 0; j < BULLETMAX; j++)
				{
					EnemyBulletUserC[j] = { false };
					UserBulletEnemyC[j] = { false };
				}
				if (score > highScore)
				{
					fp = fopen("./resource/highScore.txt", "wt");
					if (fp != NULL)
					{
						fprintf(fp, "%lld", score);
						fclose(fp);
						fp = NULL;
					}
					highScore = score;
				}
				score = 0;


				mainbg->Onscreen = true;
				Mix_PlayMusic(backgroundSound, -1);
				goto LMAINBG;
				break;
			}

		}
		SDL_RenderPresent(renderer);

			
		/*FPS 2*/
		frameTime = (Uint32)SDL_GetTicks64() - frameStart;
		if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);

		/*COUNT UPDATE*/
		MainCount++;


	}

	/*SAVING HIGH SCORE*/
	if (score > highScore)
	{
		fp = fopen("./resource/highScore.txt", "wt");
		if (fp != NULL)
		{
			fprintf(fp, "%lld", score);
			fclose(fp);
			fp = NULL;
		}
	}

	/*DELAY*/


	/*EXIT*/
	autoFree(Ocean);
	autoFree(Cloud);
	autoFree(User);
	autoFree(UserBullet);
	autoFree(Enemy);
	autoFree(EnemyBullet);

	// SDL
	// music
	Mix_FreeChunk(lunchBulletEffect);
	Mix_FreeChunk(gameoverEffect);
	Mix_FreeChunk(getScore);
	Mix_FreeMusic(backgroundSound);
	Mix_CloseAudio();
	// default
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

void* TTF_AutoPrinting(const char* text, int size, SDL_Renderer* renderer, uint8_t R, uint8_t G, uint8_t B, uint8_t A, int posX, int posY)
{
	TTF_Font* font = TTF_OpenFont("./resource/NeoDunggeunmoPro-Regular.ttf", size);
	if (font == NULL) {
		return NULL;
	}

	SDL_Color fontColor = { R, G, B, A };

	SDL_Surface* surface = TTF_RenderText_Blended(font, text, fontColor);
	if (surface == NULL) {
		TTF_CloseFont(font);
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) {
		return NULL;
	}

	SDL_Rect rect = { posX, posY, surface->w, surface->h };

	SDL_RenderCopy(renderer, texture, NULL, &rect);

	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	return nullptr;
}