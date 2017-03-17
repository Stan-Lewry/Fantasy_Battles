#ifndef GAME_HEADER
#define GAME_HEADER


#include "SDLUtils.h"
#include "Input.h"
#include "World.h"
#include "Renderer.h"
#include "Character.h"
#include "UI.h"

class Game{
public:
	Game();

	
	void initCharacters();
	bool selectFriendlyCharacter(int mouseX, int mouseY);
	bool selectTargetCharacter(int mouseX, int mouseY);

	bool characterInThatPosition(int worldX, int worldY);

	void attack(int mouseX, int mouseY);

	void doCombat(Character* friendly, Character* target);
	void update(InputState inputState);
	void changeState(GameState newState);
	void switchCharacterLists();
	void getRanges(Character* c);
	void endTurn();
	void gameLoop();
private:


	Uint32 oldTime;
	Uint32 currentTime;
	float ftime;
	float timer;
	int frames;

	SDLUtils* sdlUtils;
	Input* input;
	Renderer* renderer;	
	UI* ui;
	World* world;
	Character* activeCharacterList[teamSize];
	Character* inactiveCharacterList[teamSize];
	Character* selectedFriendlyCharacter;
	Character* selectedTargetCharacter;

	//AnimationObject* animationObjects[5]; 

	AnimationObject* testAnimation;

	bool globalRunning;
	

	GameState currentState = MAINMENU;
	
	SDL_Texture* mapTileTexture = NULL;
	
	int spriteSize = 32;
	//SDL_Event evnt;
	int tileSize = 64;

	int scrollSpeed = 10;
	
};

#endif
