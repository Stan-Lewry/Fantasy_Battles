#include "Game.h"

Game* game;

int main(int argv, char* argc[]){
	game = new Game();
	game->gameLoop();
	return 0;
}