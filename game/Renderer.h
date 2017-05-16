// Renderer Class
// This class abstracts all rendering from the game class
// Essentially just a wrapper around SDL_RenderCopy calls
// Details on implemetation can be found in Renderer.cpp

#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include "SDLUtils.h"
//#include "Game.h"
//#include "World.h"
#include "Character.h"




class Renderer{
public:
	Renderer(SDL_Renderer* _rend);

	void renderGame(mapTile map[mapH][mapW], int mapWidth, int mapHeight, Character* renderableCharacters1[teamSize], 
		Character* renderableCharacters2[teamSize], Character* currentCharacter, 
		UIElement renderableUIElements[1], AnimationObject* animationObject, int mousex, int mouseY, bool paused,
		char* currentTeam, int turnNo);

	void renderCharacters(Character* renderableCharacters[teamSize], mapTile map[mapW][mapH], int mapWidth, int mapHeight);
	void renderStatusBars(Character* renderableCharacters[teamSize]);
	void renderUI(UIElement renderableUIElements[1], Character* currentCharacter, char* currentTeam, int turnNo);
	void renderWorld(mapTile map[mapH][mapW],int mapWidth, int mapHeight, Character* currentCharacter, Character* charList1[teamSize], Character* charList2[teamSize]);
	void renderMapBackground();
	void renderMapForeground();
	void renderCursor(int mouseX, int mouseY);
	void renderText(const char* text, TTF_Font* font, int x, int y, int r, int g, int b);
	void renderAnimationObjects(AnimationObject* object);
	void renderPauseMenu(UIElement renderableUIElements[4], int mouseX, int mouseY);

	void renderMainMenu(UIElement renderableUIElements[1], int mouseX, int mouseY);
	void renderStageSelect(UIElement renderableUIElements[4], int mouseX, int mouseY);

	void initTextures();
	void initFonts();
	SDL_Texture* loadPNG(char path[]);

	
	void addAnimationObject(int screenX, int screenY, AnimationType animType, std::string text);

	int getRenderOffsetX();
	int getRenderOffsetY();

	int getMapSpaceOffset();

	void addOffsetX(int a);
	void addOffsetY(int a);

private:
	SDL_Renderer* rend;
	SDL_Texture* worldSpriteSheet;
	SDL_Texture* characterSpriteSheet;
	SDL_Texture* uiSpriteSheet;
	SDL_Texture* titleScreen;
	SDL_Texture* mainMenuButtons;
	SDL_Texture* stageSelectScreen;
	SDL_Texture* stageSelectButtons;
	SDL_Texture* pauseScreen;
	SDL_Texture* pauseButtons;
	SDL_Texture* swordAnimationSheet;
	SDL_Texture* fireAnimationSheet;
	SDL_Texture* attackAnimSheet = NULL;
	SDL_Texture* foreground = NULL;

	TTF_Font* font = NULL;
	TTF_Font* dmgFont = NULL;

	int spriteSize = 64;
	int worldSpriteSize = 128;
	int tileSize = 128;
	int renderOffsetX = 640;
	int renderOffsetY = -100;


	int mapSpaceOffset = 200;

	Uint32 oldTime;
	Uint32 currentTime;
	float ftime;
	float timer;
	int frames;

	std::vector<AnimationObject> animationStack;

	char* blueTeam = "Blue Team";
	char* redTeam = "Red Team";

};

#endif
