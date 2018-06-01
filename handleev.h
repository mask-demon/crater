#ifndef _CRATER_HANDLEEV
#define _CRATER_HANDLEEV
#include <SDL2/SDL.h>

#define FLAG_KEY_RIGHT 1
#define FLAG_KEY_DOWN 2
#define FLAG_KEY_LEFT 4
#define FLAG_KEY_UP 8
#define FLAG_KEY_RETURN 16
#define FLAG_KEY_ESC 32

extern int evw_keySenseFlag;
extern int evr_keyDown;
extern bool evr_quit;
extern int evm_keyPressed;
extern SDL_mutex *mtxEM, *mtxKeyFlag;
extern SDL_sem *semEM;

int handleEvents(void *);

#endif
