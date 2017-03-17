
#include "Game.h"




Game* game;


int main(int argv, char* argc[]){

	game = new Game();
	SDL_ShowCursor(SDL_DISABLE);

	//game->initMap();
	
	//game->buildMapFromTypes();
	//game->initTextures();

	game->gameLoop();

	return 0;
}