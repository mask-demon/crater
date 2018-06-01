#include "handleev.h"
#include <SDL2/SDL.h>
#include <cstdio>

int evw_keySenseFlag;
int evr_keyDown;
bool evr_quit;
int evm_keyPressed;
SDL_mutex *mtxEM, *mtxKeyFlag;
SDL_sem *semEM;

int handleEvents(void *V) {
	SDL_Event ev1;
	printf("Successfully opened a new thread.\n");
	fflush(stdout);
	bool ifCondEM = false;
	while(true) {
		SDL_WaitEvent(&ev1);
		printf(" !%d\n", evw_keySenseFlag);
		fflush(stdout);
		if(ev1.type == SDL_QUIT) {
			evr_quit = true;
			ifCondEM = true;
//			exit(0);
		} else if(ev1.type == SDL_KEYDOWN) {
			printf(" !\n");
			fflush(stdout);
			switch(ev1.key.keysym.scancode) {
				case SDL_SCANCODE_RETURN:
					evr_keyDown |= FLAG_KEY_RETURN;
					if(evw_keySenseFlag | FLAG_KEY_RETURN) {
						SDL_LockMutex(mtxKeyFlag);
						evm_keyPressed |= FLAG_KEY_RETURN;
						SDL_UnlockMutex(mtxKeyFlag);
						ifCondEM = true;
					}
					break;
				case SDL_SCANCODE_ESCAPE:
					evr_keyDown |= FLAG_KEY_ESC;
					if(evw_keySenseFlag | FLAG_KEY_ESC) {
						SDL_LockMutex(mtxKeyFlag);
						evm_keyPressed |= FLAG_KEY_ESC;
						SDL_UnlockMutex(mtxKeyFlag);
						ifCondEM = true;
					}
					break;
				case SDL_SCANCODE_UP:
					evr_keyDown |= FLAG_KEY_UP;
					if(evw_keySenseFlag | FLAG_KEY_UP) {
						SDL_LockMutex(mtxKeyFlag);
						evm_keyPressed |= FLAG_KEY_UP;
						SDL_UnlockMutex(mtxKeyFlag);
					}
					ifCondEM = true;
					break;
				case SDL_SCANCODE_DOWN:
					evr_keyDown |= FLAG_KEY_DOWN;
					if(evw_keySenseFlag | FLAG_KEY_DOWN) {
						SDL_LockMutex(mtxKeyFlag);
						evm_keyPressed |= FLAG_KEY_DOWN;
						SDL_UnlockMutex(mtxKeyFlag);
					}
					ifCondEM = true;
					break;
				case SDL_SCANCODE_RIGHT:
					evr_keyDown |= FLAG_KEY_RIGHT;
					if(evw_keySenseFlag | FLAG_KEY_RIGHT) {
						SDL_LockMutex(mtxKeyFlag);
						evm_keyPressed |= FLAG_KEY_RIGHT;
						SDL_UnlockMutex(mtxKeyFlag);
					}
					ifCondEM = true;
					break;
				case SDL_SCANCODE_LEFT:
					evr_keyDown |= FLAG_KEY_LEFT;
					if(evw_keySenseFlag | FLAG_KEY_LEFT) {
						SDL_LockMutex(mtxKeyFlag);
						evm_keyPressed |= FLAG_KEY_LEFT;
						SDL_UnlockMutex(mtxKeyFlag);
					}
					ifCondEM = true;
					break;
				default:
					;
				
			}
		}
		if(ifCondEM) {
			printf("send signal.\n");
			fflush(stdout);
			SDL_LockMutex(mtxEM);
			SDL_SemPost(semEM);
			SDL_UnlockMutex(mtxEM);
			ifCondEM = false;
		}
	}
}
