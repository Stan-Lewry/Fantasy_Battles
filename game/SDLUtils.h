#ifndef SDLUTILS_HEADER
#define SDLUTILS_HEADER

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>




#define screenW 960
#define screenH 960

#define screenW 1280
#define screenH 720

#define teamSize 3



const int mapW = 15;
const int mapH = 15;
const int mapSize = mapW * mapH;


//int renderOffsetX = -200;
//int renderOffsetY = -200;

//int spriteSize = 32;
//int tileSize = 32;
//bool globalRunning = true;


struct mapTile{
	int screenX;
	int screenY;
	int worldX;
	int worldY;
	int worldZ;
	int typeX;
	int typeY;
	bool blocked;
	bool selected;
	bool moveRange;
	bool attackRange;
	bool exists;
	//bool rangedAttackRange
};



enum UIAction{NO_ACTION, A, B, C , CHANGESTATE_GAMEPLAY, QUIT};
enum GameState{MAINMENU, LEVELSELECT, GAMEPLAY};
enum AnimationType{DMG_NO, SWORD_ATTACK, MAGIC_ATTACK};

struct AnimationObject{
	int screenX, screenY, width, height, animationFrame, endFrame;
	AnimationType animType;
	bool dead;
	float frameSpeed;
	float frameTimer;
	//const char* text;
	std::string text;
};

struct UIElement{
	int screenX;
	int screenY;
	int width;
	int height;
	int sourceX;
	int sourceY;
	//int animationFrame;
	bool hover;
	UIAction action;
};

/*
template <typename T>
std::string to_string(T value){
	std::ostringstream os;
	os << value;
	return os.str();
}
*/
class SDLUtils{
public: 
	SDL_Window* window;
	SDL_Renderer* rend;

	bool initSDL();
	//SDL_Texture* loadPNG(char path[]);
};

#endif
