#include "SDLUtils.h"

bool SDLUtils::initSDL(){
	SDL_Init(SDL_INIT_EVERYTHING);

	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	window = SDL_CreateWindow("Fatnasty Battles: Turbow Edition", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, SDL_WINDOW_SHOWN);
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

//SDL_Texture* SDLUtils::loadPNG(char path[]){
//	printf("loading: %s\n", path);
//	SDL_Surface* tempSurface = IMG_Load(path);
//	SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, tempSurface);
//	SDL_FreeSurface(tempSurface);
//	printf("Done!\n");
//	return texture;
//}
