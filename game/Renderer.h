#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include "SDLUtils.h"
//#include "Game.h"
//#include "World.h"
#include "Character.h"




class Renderer{
public:
	Renderer(SDL_Renderer* _rend);
	void renderGame(mapTile map[mapH][mapW], Character* renderableCharacters1[teamSize], 
		Character* renderableCharacters2[teamSize], Character* currentCharacter, 
		UIElement renderableUIElements[1], AnimationObject* animationObject, int mousex, int mouseY);

	void renderCharacters(Character* renderableCharacters[teamSize], mapTile map[mapW][mapH]);
	void renderStatusBars(Character* renderableCharacters[teamSize]);
	void renderUI(UIElement renderableUIElements[1], Character* currentCharacter);
	void renderWorld(mapTile map[mapH][mapW], Character* currentCharacter, Character* charList1[teamSize], Character* charList2[teamSize]);
	void renderMapBackground();
	void renderCursor(int mouseX, int mouseY);
	void renderText(const char* text, TTF_Font* font, int x, int y, int r, int g, int b);
	void renderAnimationObjects(AnimationObject* object);

	void renderMainMenu(UIElement renderableUIElements[1], int mouseX, int mouseY);

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
	SDL_Texture* swordAnimationSheet;
	SDL_Texture* fireAnimationSheet;
	SDL_Texture* attackAnimSheet = NULL;

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
};

#endif
