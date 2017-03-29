#include "SDLUtils.h"

// bool SDLUtils::initSDL()
// just sets up all the sdl stuff needed (window, renderer, etx)
bool SDLUtils::initSDL(){
	SDL_Init(SDL_INIT_EVERYTHING);

	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	SDL_ShowCursor(SDL_DISABLE);
	window = SDL_CreateWindow("Fantasy Battles: Turbo Edition", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, SDL_WINDOW_SHOWN);
	if (window != NULL){
		printf("Window Initialized\n");
		rend = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
		if (rend != NULL){
			printf("Renderer Initialized\n");
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}
