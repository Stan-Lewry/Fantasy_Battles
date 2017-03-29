// Game Class
// This class defines the core functionality of the game and game logic, acting as a "god class" overseeing all other
// components of the game and calling their functions when needed. As such a Game contains instances of each class in the game
// At the core of this class is the game loop, which specifies functionality based on the current game state
// game loop processes inputs, updates the game, and finally passes everything to the renderer to draw the frame
// Details on the implementation of each funciton can be found in Game.cpp

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

	void processInputs(InputState inputState);
	void processInputsGameplay(InputState inputState);
	void processInputsMainMenu(InputState inputState);

	void doCombat(Character* friendly, Character* target);
	void update();
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
	bool characterAnimating = false;

	GameState currentState = MAINMENU;
	
	SDL_Texture* mapTileTexture = NULL;
	
	int spriteSize = 32;
	//SDL_Event evnt;
	int tileSize = 64;

	int scrollSpeed = 5;
	
};

#endif
