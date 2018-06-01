#include "mymath.h"
#include "stage.h"
#include "handleev.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <ctime>
using namespace std;

#define PATH_FONT "keifont.ttf"
#define WIDTH 640
#define HIGHT 400
#define BUFNUM 7
#define ICONSIZE 48
#define IMGSIZE 32
#define DIVNUM 4
#define MAPLSIZE 4
#define MAPL_COLOR ((SDL_Color){225, 0, 0, 0})

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Surface *surface;
TTF_Font *font;
Mix_Music *bgm;
SDL_Thread *trdEv, *trdCent;
SDL_Event ev;

int init() {
	srand((unsigned)time(NULL));
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	Mix_Init(MIX_INIT_MID | MIX_INIT_MP3);
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)) {
		fprintf(stderr, "Could not open an audio device: %s\n", Mix_GetError());
		return 1;
	}
	TTF_Init();
	window = SDL_CreateWindow(
		"Crater",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH,
		HIGHT,
		0);
	if(!window) {
		fprintf(stderr, "Could not create a window: %s\n", SDL_GetError());
		return 1;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer) {
		fprintf(stderr, "Could not create a rendere: %s\n", SDL_GetError());
		return 2;
	}
	font = TTF_OpenFont(PATH_FONT, 64);
	if(!font) {
		fprintf(stderr, "Could not open a ttf file: %s\n", TTF_GetError());
	}
	evw_keySenseFlag = 0;
	evr_quit = false;
	evr_keyDown = 0;
	evm_keyPressed = 0;
	mtxEM = SDL_CreateMutex();
	mtxKeyFlag = SDL_CreateMutex();
	if(!mtxKeyFlag) {
		fprintf(stderr, "Could not create a mutex: %s\n", SDL_GetError());
		return 3;
	}
	semEM = SDL_CreateSemaphore(0);
	if(!semEM) {
		fprintf(stderr, "Coudl not create a semaphore.\n");
		return 4;
	}
//	trdCent = SDL_CreateThread(center, "center", 0);
//	trdEv = SDL_CreateThread(handleEvents, "handleEvents", 0);
	return 0;
}

void quit() {
	SDL_DetachThread(trdEv);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	TTF_CloseFont(font);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void printBack(SDL_Surface *S) {
	if(S) {
		texture = SDL_CreateTextureFromSurface(renderer, S);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
	} else
		SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void printString(string S, SDL_Color C = (SDL_Color){255, 255, 255, 255}) {
	int rem = 0;
	vector<string> pos;
	for(unsigned i = 0; S[i]; i++) {
		if(S[i] == '\n') {
			pos.push_back(S.substr(rem, i - rem));
			rem = i + 1;
		}
	}
	pos.push_back(S.substr(rem, S.size() - rem));
	int y = HIGHT;
	SDL_Surface *surface;
	for(vector<string>::const_iterator it = pos.end(); it != pos.begin();) {
		it--;
		surface = TTF_RenderUTF8_Blended(font, it->c_str(), C);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		int iw, ih;
		SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);
		SDL_Rect dstR = (SDL_Rect){0, y - ih, iw, ih};
		y -= ih;
		SDL_RenderCopy(renderer, texture, NULL, &dstR);
		SDL_RenderPresent(renderer);
	}
}

void printString_c(string S, SDL_Color C = (SDL_Color){255, 255, 255, 255}) {
	SDL_Surface *surface;
	surface = TTF_RenderUTF8_Blended(font, S.c_str(), C);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	int iw, ih;
	SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);
	SDL_Rect dstR = (SDL_Rect){(WIDTH - iw) / 2, (HIGHT - ih) / 2, iw, ih};
	SDL_RenderCopy(renderer, texture, NULL, &dstR);
	SDL_RenderPresent(renderer);
}

int firstmenu() {
	surface = IMG_Load("imgs/Lybia.png");
	printBack(surface);
	printString("Enter: はじめ\nEsc: おわり");

	evw_keySenseFlag = FLAG_KEY_RETURN | FLAG_KEY_ESC;
	SDL_LockMutex(mtxKeyFlag);
	evm_keyPressed = 0;
	SDL_UnlockMutex(mtxKeyFlag);
	printf("%d\n", SDL_LockMutex(mtxEM));
	fflush(stdout);
	int trt = 0;
	while(!trt) {
//		SDL_SemWait(semEM);
		printf("$\n");
		fflush(stdout);
		/*
		if(SDL_PollEvent(&ev)) {
			if(ev.type == SDL_QUIT) {
				trt = -1;
			}
		}
		*/
		SDL_Delay(100);
		while(SDL_PollEvent(&ev)) {
			if(ev.type == SDL_QUIT || (ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
				return -1;
			}
			if(ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				Mix_HaltMusic();
				return 1;
			}
		}
	/*
		if(evr_quit || (evm_keyPressed & FLAG_KEY_ESC)) {
			trt = -1;
		} else if(evm_keyPressed & FLAG_KEY_RETURN) {
			printf("Enter pressed\n");
			fflush(stdout);
			trt = 1;
		}
	*/
	}
	SDL_LockMutex(mtxKeyFlag);
	evm_keyPressed = 0;
	SDL_UnlockMutex(mtxKeyFlag);
	evw_keySenseFlag = 0;
	SDL_UnlockMutex(mtxEM);
	return trt;
}

int town() {
	surface = IMG_Load("imgs/town.jpg");
	bgm = Mix_LoadMUS("songs/town.mid");
	Mix_PlayMusic(bgm, -1);
	printBack(surface);
	printString("ここは町。\nEnter: タワーに行く\nEsc: おわり");

	while(true) {
		SDL_Delay(100);
		while(SDL_PollEvent(&ev)) {
			if(ev.type == SDL_QUIT || (ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
				return -1;
			}
			if(ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				Mix_HaltMusic();
				return 2;
			}
		}
	}
}

int dungeon() {
	bgm = Mix_LoadMUS("songs/stage_1.mid");
	Mix_PlayMusic(bgm, -1);
	SDL_Surface *icons[3];
	icons[0] = IMG_Load("imgs/twr_wall.png");
	icons[1] = IMG_Load("imgs/twr_floor.png");
	icons[2] = IMG_Load("imgs/twr_stairs.png");
	SDL_Surface *sfcplayer = IMG_Load("imgs/player.png");
	SDL_Texture *texstage, *texmap, *texplayer = SDL_CreateTextureFromSurface(renderer, sfcplayer);
	SDL_Rect rctplayer = (SDL_Rect){WIDTH - (HIGHT + ICONSIZE) / 2, (HIGHT - ICONSIZE) / 2, ICONSIZE, ICONSIZE};
	SDL_Rect stgR = (SDL_Rect){WIDTH - HIGHT, 0, HIGHT, HIGHT};
	SDL_Rect mapR = (SDL_Rect){0, HIGHT * 2 - WIDTH, WIDTH - HIGHT, WIDTH - HIGHT};
	const int SIGHT = (HIGHT + ICONSIZE) / 2 / ICONSIZE;
	
	for(int i = 0; i < 1; i++) {
		int H = 40, W = 40, STEPS = 25;
		int stage[H][W];
		bool room[H][W];
		SDL_FreeSurface(surface);
		int rmask, gmask, bmask, amask;
		/*
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
		#endif
		*/
		
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0x00ff0000;
			gmask = 0x0000ff00;
			bmask = 0x000000ff;
		#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
		#endif
		amask = 0;
		surface = SDL_CreateRGBSurface(0, IMGSIZE * (W + BUFNUM * 2), IMGSIZE * (H + BUFNUM * 2), 24, rmask, gmask, bmask, amask);
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask <<= 8;
			gmask <<= 8;
			bmask <<= 8;
			amask = 0x000000ff;
		#else
			amask = 0xff000000;
		#endif
		SDL_Surface *sfcMap = SDL_CreateRGBSurface(0, MAPLSIZE * (W + BUFNUM * 2) + 1, MAPLSIZE * (H + BUFNUM * 2) + 1, 32, rmask, gmask, bmask, amask);
		if(surface == NULL) {
			fprintf(stderr, "Could not make a new surface.\n");
		}
		for(int j = 0; j < 10; j++) {
			printBack(NULL);
			SDL_Delay(200);
			while(SDL_PollEvent(&ev)) {
				if(ev.type == SDL_QUIT) {
					return -1;
				}
			}
			char str[20];
			sprintf(str, "タワー%2d階", 10 * i + j + 1);
			printString_c(str);
			makestage(stage[0], room[0], H, W, STEPS);
			int y, x;
			do {
				y = rand() % H;
				x = rand() % W;
			} while(!room[y][x]);
			stage[y][x] = 2;
			do {
				y = rand() % H;
				x = rand() % W;
			} while(!room[y][x] || stage[y][x] == 2);
			SDL_FillRect(sfcMap, NULL, SDL_MapRGBA(sfcMap->format, 0, 0, 0, 255));
			int msy = y, mty = y, msx = x, mtx = x;
			for(int my = y - SIGHT; my <= y + SIGHT; my++) {
				for(int mx = x - SIGHT; mx <= x + SIGHT; mx++){
					if(my != y + SIGHT)
						if(inside_in(my, mx, H, W) && inside_in(my + 1, mx, H, W) && ((bool)stage[my][mx] ^ (bool)stage[my + 1][mx])) {
							SDL_Rect lineR = (SDL_Rect){mx * MAPLSIZE, (my + 1) * MAPLSIZE, MAPLSIZE + 1, 1};
							SDL_FillRect(sfcMap, &lineR, SDL_MapRGB(sfcMap->format, 255, 0, 0));
							inmin(msy, my);
							inmax(mty, my + 1);
							inmin(msx, mx);
							inmax(mtx, mx);
						}
					if(mx != x + SIGHT)
						if(inside_in(my, mx, H, W) && inside_in(my, mx + 1, H, W) && ((bool)stage[my][mx] ^ (bool)stage[my][mx + 1])) {
							SDL_Rect lineR = (SDL_Rect){(mx + 1) * MAPLSIZE, my * MAPLSIZE, 1, MAPLSIZE + 1};
							SDL_FillRect(sfcMap, &lineR, SDL_MapRGB(sfcMap->format, 255, 0, 0));
							inmin(msy, my);
							inmax(mty, my);
							inmin(msx, mx);
							inmax(mtx, mx + 1);
						}
				}
			}
			while(SDL_PollEvent(&ev)) {
				if(ev.type == SDL_QUIT) {
					return -1;
				}
			}
			for(int iy = 0; iy < BUFNUM; iy++) {
				for(int ix = 0; ix < BUFNUM * 2 + W; ix++) {
					SDL_Rect dstR = (SDL_Rect){ix * IMGSIZE, iy * IMGSIZE, IMGSIZE, IMGSIZE};
					SDL_BlitSurface(icons[0], NULL, surface, &dstR);
					dstR.y += IMGSIZE * (H + BUFNUM);
					SDL_BlitSurface(icons[0], NULL, surface, &dstR);
				}
			}
			for(int iy = 0; iy < H; iy++) {
				for(int ix = 0; ix < BUFNUM; ix++) {
					SDL_Rect dstR = (SDL_Rect){ix * IMGSIZE, (iy + BUFNUM) * IMGSIZE, IMGSIZE, IMGSIZE};
					SDL_BlitSurface(icons[0], NULL, surface, &dstR);
					dstR.x += IMGSIZE * (W + BUFNUM);
					SDL_BlitSurface(icons[0], NULL, surface, &dstR);
				}
			}
			for(int iy = 0; iy < H; iy++) {
				for(int ix = 0; ix < W; ix++) {
					SDL_Rect dstR = (SDL_Rect){(ix + BUFNUM) * IMGSIZE, (iy + BUFNUM) * IMGSIZE, IMGSIZE, IMGSIZE};
					SDL_BlitSurface(icons[stage[iy][ix]], NULL, surface, &dstR);
				}
			}
			while(SDL_PollEvent(&ev)) {
				if(ev.type == SDL_QUIT) {
					return -1;
				}
			}
			SDL_Delay(1000);
			while(SDL_PollEvent(&ev)) {
				if(ev.type == SDL_QUIT) {
					return -1;
				}
			}
			printBack(NULL);
			SDL_Delay(200);
			while(SDL_PollEvent(&ev)) {
				if(ev.type == SDL_QUIT) {
					return -1;
				}
			}
			texstage = SDL_CreateTextureFromSurface(renderer, surface);
			
			int iw, ih;
			SDL_QueryTexture(texstage, NULL, NULL, &iw, &ih);
			const int xlength = iw / (W + BUFNUM * 2), ylength = ih / (H + BUFNUM * 2);
			SDL_Rect srcR = (SDL_Rect){0, 0, HIGHT * xlength / ICONSIZE, HIGHT * ylength / ICONSIZE};
			bool next = false;
			while(!next) {
				srcR.y = ((2 * (y + BUFNUM) + 1) * ylength - HIGHT * ylength / ICONSIZE) / 2;
				srcR.x = ((2 * (x + BUFNUM) + 1) * xlength - HIGHT * xlength / ICONSIZE) / 2;
				texmap = SDL_CreateTextureFromSurface(renderer, sfcMap);
				SDL_RenderCopy(renderer, texstage, &srcR, &stgR);
				SDL_RenderCopy(renderer, texplayer, NULL, &rctplayer);
				SDL_RenderCopy(renderer, texmap, NULL, &mapR);
				SDL_RenderPresent(renderer);
				if(stage[y][x] == 2) {
					SDL_Delay(200);
					break;
				}
				SDL_Delay(30);
				bool f = true;
				while(f) {
					while(SDL_PollEvent(&ev)) {
						if(ev.type == SDL_QUIT) {
							return -1;
						}
						if(ev.type == SDL_KEYDOWN) {
							f = false;
							break;
						}
					}
					if(!f) {
						int dist = -1;
						f = true;
						switch(ev.key.keysym.scancode) {
							case SDL_SCANCODE_UP:
								dist = 3;
								break;
							case SDL_SCANCODE_DOWN:
								dist = 1;
								break;
							case SDL_SCANCODE_RIGHT:
								dist = 0;
								break;
							case SDL_SCANCODE_LEFT:
								dist = 2;
								break;
							default:
								;
						}
						if(dist >= 0) {
							int ny = y + NY[dist], nx = x + NX[dist];
							if(stage[ny][nx]) {
								int my = ny + NY[dist] * SIGHT, mx = nx + NX[dist] * SIGHT;
								for(int k = -SIGHT; k <= SIGHT; k++) {
									const int sy = my + NY[dist + 1] * k, sx = mx + NX[dist + 1] * k;
									if(!inside_in(sy, sx, H, W))
										continue;
									if(k != SIGHT) {
										const int ty = sy + NY[dist + 1], tx = sx + NX[dist + 1];
										if(inside_in(ty, tx, H, W) && ((bool)stage[sy][sx] ^ (bool)stage[ty][tx])) {
											SDL_Rect lineR;
											if(dist & 1)
												lineR = (SDL_Rect){max(sx, tx) * MAPLSIZE, min(sy, ty) * MAPLSIZE, 1, MAPLSIZE + 1};
											else
												lineR = (SDL_Rect){min(sx, tx) * MAPLSIZE, max(sy, ty) * MAPLSIZE, MAPLSIZE + 1, 1};
											SDL_FillRect(sfcMap, &lineR, SDL_MapRGB(sfcMap->format, 255, 0, 0));
											inmin(msy, min(sy, ty));
											inmax(mty, max(sy, ty));
											inmin(msx, min(sx, tx));
											inmax(mtx, max(sx, tx));
										}
									}
									const int ty = sy - NY[dist], tx = sx - NX[dist];
									if(inside_in(ty, tx, H, W) && ((bool)stage[sy][sx] ^ (bool)stage[ty][tx])) {
										SDL_Rect lineR;
										if(dist & 1)
											lineR = (SDL_Rect){min(sx, tx) * MAPLSIZE, max(sy, ty) * MAPLSIZE, MAPLSIZE + 1, 1};
										else
											lineR = (SDL_Rect){max(sx, tx) * MAPLSIZE, min(sy, ty) * MAPLSIZE, 1, MAPLSIZE + 1};
										SDL_FillRect(sfcMap, &lineR, SDL_MapRGB(sfcMap->format, 255, 0, 0));
										inmin(msy, min(sy, ty));
										inmax(mty, max(sy, ty));
										inmin(msx, min(sx, tx));
										inmax(mtx, max(sx, tx));
									}
								}
								for(int k = 1; k < DIVNUM; k++) {
									srcR.y += NY[dist] * ylength / DIVNUM;
									srcR.x += NX[dist] * xlength / DIVNUM;
									SDL_RenderCopy(renderer, texstage, &srcR, &stgR);
									SDL_RenderCopy(renderer, texplayer, NULL, &rctplayer);
									SDL_RenderPresent(renderer);
									SDL_Delay(30);
								}
								y = ny;
								x = nx;
								f = false;
							}
						}
					} else
						SDL_Delay(100);
				}
			}
		}
	}
	Mix_HaltMusic();
	return 1;
}

int main(int argc, char *argv[]) {
	if(init())
		return 0;
	
	int mode = firstmenu();
	while(true) {
		if(mode == -1)
			break;
		else if(mode == 1)
			mode = town();
		else if(mode == 2)
			mode = dungeon();
		else
			break;
		printBack(NULL);
		SDL_Delay(200);
	}
	quit();
	return 0;
}
