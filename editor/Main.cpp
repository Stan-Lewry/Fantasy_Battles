#include <iostream>
#include <fstream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define screenW 1280
#define screenH 720
#define mapW 15
#define mapH 15
#define tileSize 64
#define screenTileSize 128
#define spriteSize 32
#define worldSpriteSize 128
#define paletteSpriteSize 32

SDL_Window* window;
SDL_Renderer* rend;
SDL_Texture* tiles;
SDL_Texture* ui;
SDL_Texture* paletteSheet;
SDL_Event evnt;
TTF_Font* font = NULL; 

int renderOffsetX = 0; 
int renderOffsetY = 0;

int paletteX = screenW - 340;
int paletteY = 35;
//int palettespriteSize = 32;

int currentType = 1;

bool gRunning = false;

struct tile{
	int worldX;
	int worldY;
	int worldZ;
	int screenX;
	int screenY;
	int type;
};

struct paletteButton{
	int x;
	int y;
	int w;
	int h;
	int tileType;
};

struct Button{
	int x, y, w, h;
};

Button trash = { screenW - 108 - 32, screenH - 55 - 32, 32, 32 };
Button save = { 1076, screenH - 55 - 32, 32, 32 };
Button exit_program = { 1012, screenH - 55 - 32, 32, 32 };

tile map[mapH][mapW];

paletteButton paletteButtons[10][10]; 

bool initSDL(){
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	font = TTF_OpenFont("Fonts/font.ttf", 22);
	window = SDL_CreateWindow("Level Editor", 0, 0, screenW, screenH, SDL_WINDOW_SHOWN);
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

SDL_Texture* loadPNG(char* path){
	printf("loading: %s\n", path);
	SDL_Surface* tempSurface = IMG_Load(path);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, tempSurface);
	SDL_FreeSurface(tempSurface);
	printf("Done!\n");
	return texture;
}

void initTextures(){
	tiles = loadPNG("Assets/iso_tiles_large.png");
	paletteSheet = loadPNG("Assets/2d_tiles.png");
	ui = loadPNG("Assets/UI.png");
}

void initMap(){
	for (int i = 0; i < mapH; i++){
		for (int j = 0; j < mapW; j++){
			tile newTile = { j, i, 0, j * tileSize, i * tileSize, 1 };
	

			map[i][j] = newTile;
		}
	}
}

void initPaletteButtons(){
	int type = 0;
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			paletteButton pb = { j * spriteSize + paletteX, i * spriteSize + paletteY, spriteSize, spriteSize, type };
			paletteButtons[i][j] = pb;
			type++; 
		}
	}
}

void renderSelectedTile(){
	SDL_Rect sRect = { 0, 0, spriteSize, spriteSize };
	SDL_Rect dRect = { screenW - 230, screenH - 295, 100, 100 };
	switch (currentType){
	case 0:
		sRect.x = 0 * spriteSize;
		sRect.y = 0 * spriteSize;
		break;
	case 1:
		sRect.x = 1 * spriteSize;
		sRect.y = 0 * spriteSize;
		break;
	case 2:
		sRect.x = 2 * spriteSize;
		sRect.y = 0 * spriteSize;
		break;
	case 3:
		sRect.x = 3 * spriteSize;
		sRect.y = 0 * spriteSize;
		break;
	case 4:
		sRect.x = 4 * spriteSize;
		sRect.y = 0 * spriteSize;
		break;
	case 5:
		sRect.x = 5 * spriteSize;
		sRect.y = 0 * spriteSize;
		break;
	case 6:
		sRect.x = 6 * spriteSize;
		sRect.y = 0 * spriteSize;
		break;
	case 7:
		sRect.x = 7 * spriteSize;
		sRect.y = 0 * spriteSize;
		break;
	case 8:
		sRect.x = 8 * spriteSize;
		sRect.y = 0 * spriteSize;
		break;
	case 9:
		sRect.x = 9 * spriteSize;
		sRect.y = 0 * spriteSize;
		break;
	case 10:
		sRect.x = 0 * spriteSize;
		sRect.y = 1 * spriteSize;
		break;
	case 11:
		sRect.x = 1 * spriteSize;
		sRect.y = 1 * spriteSize;
		break;
	case 12:
		sRect.x = 2 * spriteSize;
		sRect.y = 1 * spriteSize;
		break;
	case 13:
		sRect.x = 3 * spriteSize;
		sRect.y = 1 * spriteSize;
		break;
	case 14:
		sRect.x = 4 * spriteSize;
		sRect.y = 1 * spriteSize;
		break;
	case 15:
		sRect.x = 5 * spriteSize;
		sRect.y = 1 * spriteSize;
		break;
	case 16:
		sRect.x = 6 * spriteSize;
		sRect.y = 1 * spriteSize;
		break;
	case 20:
		sRect.x = 0 * spriteSize;
		sRect.y = 2 * spriteSize;
		break;
	case 21:
		sRect.x = 1 * spriteSize;
		sRect.y = 2 * spriteSize;
		break;
	case 22:
		sRect.x = 2 * spriteSize;
		sRect.y = 2 * spriteSize;
		break;
	case 23:
		sRect.x = 3 * spriteSize;
		sRect.y = 2 * spriteSize;
		break;
	case 24:
		sRect.x = 4 * spriteSize;
		sRect.y = 2 * spriteSize;
		break;
	case 25:
		sRect.x = 5 * spriteSize;
		sRect.y = 2 * spriteSize;
		break;
	case 26:
		sRect.x = 6 * spriteSize;
		sRect.y = 2 * spriteSize;
		break;
	case 27:
		sRect.x = 7 * spriteSize;
		sRect.y = 2 * spriteSize;
		break;
	case 28:
		sRect.x = 8 * spriteSize;
		sRect.y = 2 * spriteSize;
		break;
	case 29:
		sRect.x = 9 * spriteSize;
		sRect.y = 2 * spriteSize;
		break;
	case 30:
		sRect.x = 0 * spriteSize;
		sRect.y = 3 * spriteSize;
		break;
	case 31:
		sRect.x = 1 * spriteSize;
		sRect.y = 3 * spriteSize;
		break;
	case 32:
		sRect.x = 2 * spriteSize;
		sRect.y = 3 * spriteSize;
		break;
	}
	SDL_RenderCopy(rend, paletteSheet, &sRect, &dRect);
}

void renderZVaues(const char* num, int worldX, int worldY){
	SDL_Rect dest = { (worldX * tileSize) + renderOffsetX, (worldY * tileSize) + renderOffsetY, 0, 0 };
	TTF_SizeText(font, num, &dest.w, &dest.h);
	SDL_Color col = { 255, 0, 0 };
	SDL_Surface* tempSurf = TTF_RenderText_Solid(font, num, col);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, tempSurf);
	SDL_RenderCopy(rend, texture, NULL, &dest);
	SDL_FreeSurface(tempSurf);
	SDL_DestroyTexture(texture);
}

void renderMap(){
	SDL_Rect bg = { 0, 0, screenW, screenH };
	SDL_SetRenderDrawColor(rend, 102, 204, 255, 1);
	SDL_RenderFillRect(rend, &bg);

	SDL_Rect sRect = { 0, 0, worldSpriteSize, worldSpriteSize };
	SDL_Rect dRect = { 0, 0, screenTileSize, screenTileSize };


	for (int i = 0; i < mapH; i++){
		for (int j = 0; j < mapW; j++){
			//printf("rendering tile\n");

			if (map[i][j].type == 0){
				sRect.x = 0;
				sRect.y = 0;
			}
			if (map[i][j].type == 1){
				sRect.x = 1;
				sRect.y = 0;
			}
			if (map[i][j].type == 2){
				sRect.x = 2;
				sRect.y = 0;
			}
			if (map[i][j].type == 3){
				sRect.x = 3 ;
				sRect.y = 0;
			}
			if (map[i][j].type == 4){
				sRect.x = 4;
				sRect.y = 0;
			}
			if (map[i][j].type == 5){
				sRect.x = 5;
				sRect.y = 0;
			}
			if (map[i][j].type == 6){
				sRect.x = 6;
				sRect.y = 0;
			}
			if (map[i][j].type == 7){
				sRect.x = 7;
				sRect.y = 0;
			}
			if (map[i][j].type == 8){
				sRect.x = 8;
				sRect.y = 0;
			}
			if (map[i][j].type == 9){
				sRect.x = 9;
				sRect.y = 0;
			}
			if (map[i][j].type == 10){
				sRect.x = 0;
				sRect.y = 1;
			}
			if (map[i][j].type == 11){
				sRect.x = 1;
				sRect.y = 1;
			}
			if (map[i][j].type == 12){
				sRect.x = 2;
				sRect.y = 1;
			}
			if (map[i][j].type == 13){
				sRect.x = 3;
				sRect.y = 1;
			}
			if (map[i][j].type == 14){
				sRect.x = 4;
				sRect.y = 1;
			}
			if (map[i][j].type == 15){
				sRect.x = 5;
				sRect.y = 1;
			}
			if (map[i][j].type == 16){
				sRect.x = 6;
				sRect.y = 1;
			}
			if (map[i][j].type == 16){
				sRect.x = 6;
				sRect.y = 1;
			}
			if (map[i][j].type == 20){
				sRect.x = 0;
				sRect.y = 2;
			}
			if (map[i][j].type == 21){
				sRect.x = 1;
				sRect.y = 2;
			}
			if (map[i][j].type == 22){
				sRect.x = 2;
				sRect.y = 2;
			}
			if (map[i][j].type == 23){
				sRect.x = 3;
				sRect.y = 2;
			}
			if (map[i][j].type == 24){
				sRect.x = 4;
				sRect.y = 2;
			}
			if (map[i][j].type == 25){
				sRect.x = 5;
				sRect.y = 2;
			}
			if (map[i][j].type == 26){
				sRect.x = 6;
				sRect.y = 2;
			}
			if (map[i][j].type == 27){
				sRect.x = 7;
				sRect.y = 2;
			}
			if (map[i][j].type == 28){
				sRect.x = 8;
				sRect.y = 2;
			}
			if (map[i][j].type == 29){
				sRect.x = 9;
				sRect.y = 2;
			}
			if (map[i][j].type == 30){
				sRect.x = 0;
				sRect.y = 3;
			}
			if (map[i][j].type == 31){
				sRect.x = 1;
				sRect.y = 3;
			}
			if (map[i][j].type == 32){
				sRect.x = 2;
				sRect.y = 3;
			}

			sRect.x *= worldSpriteSize;
			sRect.y *= worldSpriteSize;

			dRect.x = (map[i][j].screenX - map[i][j].screenY) / 2;
			dRect.y = (map[i][j].screenX + map[i][j].screenY) / 4;

			//dRect.x = map[i][j].screenX;
			//dRect.y = map[i][j].screenY;

			dRect.x += renderOffsetX;
			dRect.y += renderOffsetY;

			int height = map[i][j].worldZ * 16;

			

			if(map[i][j].worldZ > 0){
				for(int p = 0; p < map[i][j].worldZ; p+=2){
					SDL_Rect tsRect = {1 * worldSpriteSize, 0, worldSpriteSize, worldSpriteSize};
					SDL_Rect tdRect = dRect;
					tdRect.y -= p * 16;
					SDL_RenderCopy(rend, tiles, &tsRect, &tdRect);
				}
			}

			/*
			
			*/
			dRect.y -= height;
			//dRect.x = map[i][j].screenX + renderOffsetX;
			//dRect.y = map[i][j].screenY + renderOffsetY;
			SDL_RenderCopy(rend, tiles, &sRect, &dRect);

			
			//renderZVaues(std::to_string(map[i][j].worldZ).c_str(), map[i][j].worldX, map[i][j].worldY); 
		}
	}

}

void renderUI(){
	SDL_Rect sRect = { 0, 0, screenW, screenH };
	SDL_Rect dRect = { 0, 0, screenW, screenH };
	SDL_RenderCopy(rend, ui, &sRect, &dRect);


	sRect = { 0, 0, 320, 320 };
	dRect = {paletteX, paletteY, 320, 320 };
	SDL_RenderCopy(rend, paletteSheet, &sRect, &dRect);
}

void renderAll(){
	SDL_RenderClear(rend);
	renderMap();
	renderUI();
	renderSelectedTile();
	SDL_RenderPresent(rend);
}



void selectTile(int inputX, int inputY){
	for (int i = mapH; i > 0; i--){
		for (int j = mapW; j > 0; j--){

			int tileHeight = map[i][j].worldZ * 16;

			int minX = (map[i][j].screenX - map[i][j].screenY) / 2;
			minX += renderOffsetX;
			minX += 32;
			minX += 32/4;
			//maxX += screenTileSize / 4;

			int minY = (map[i][j].screenX + map[i][j].screenY) / 4;
			minY += renderOffsetY;
			minY += 64;
			minX += (32 / 8) * 3;
			minY -= tileHeight;
			//maxY += (screenTileSize / 8) * 3;

			//int maxX = minX + tileSize; //+ (screenTileSize / 2);
			//int maxY = minY + tileSize;//+ (screenTileSize / 4);

			int maxX = minX + 32;
			int maxY = minY + 16;

			if(inputX >= minX && inputX < maxX){
				if(inputY >= minY && inputY < maxY){
					map[i][j].type = currentType;
				}
			}

			/*
			if (inputX >= map[i][j].screenX + renderOffsetX && inputX < map[i][j].screenX + tileSize + renderOffsetX){
				if (inputY >= map[i][j].screenY + renderOffsetY && inputY < map[i][j].screenY + tileSize + renderOffsetY){
					map[i][j].type = currentType;
				}
			}
			*/
		}
	}
}

bool cycleTileZValue(int inputX, int inputY){
	for (int i = mapH; i > 0; i--){
		for (int j = mapW; j > 0; j--){

			int tileHeight = map[i][j].worldZ * 16;
			int minX = (map[i][j].screenX - map[i][j].screenY) / 2;
			minX += renderOffsetX;
			minX += 32;
			minX += 32/4;
			//maxX += screenTileSize / 4;

			int minY = (map[i][j].screenX + map[i][j].screenY) / 4;
			minY += renderOffsetY;
			minY += 64;
			minX += (32 / 8) * 3;
			minY -= tileHeight;
			//maxY += (screenTileSize / 8) * 3;

			//int maxX = minX + tileSize; //+ (screenTileSize / 2);
			//int maxY = minY + tileSize;//+ (screenTileSize / 4);

			int maxX = minX + 32;
			int maxY = minY + 16;

			if(inputX >= minX && inputX < maxX){
				if(inputY >= minY && inputY < maxY){
					//map[i][j].type = currentType;
					if(map[i][j].worldZ < 10){
						map[i][j].worldZ += 1;
					}
					else {
						map[i][j].worldZ = 0;
					}

					return true;
				}
			}

			/*
			if (inputX >= map[i][j].screenX + renderOffsetX && inputX < map[i][j].screenX + tileSize + renderOffsetX){
				if (inputY >= map[i][j].screenY + renderOffsetY && inputY < map[i][j].screenY + tileSize + renderOffsetY){
					map[i][j].type = currentType;
				}
			}
			*/
		}
	}
	return false;
}

/*
void cycleTileZValue(int inputX, int inputY){
	for (int i = 0; i < mapH; i++){
		for (int j = 0; j < mapW; j++){
			if (inputX >= map[i][j].screenX + renderOffsetX && inputX < map[i][j].screenX + tileSize + renderOffsetX){
				if (inputY >= map[i][j].screenY + renderOffsetY && inputY < map[i][j].screenY + tileSize + renderOffsetY){
					if (map[i][j].worldZ < 15){
						map[i][j].worldZ++;
					}
					else map[i][j].worldZ = 0;
				}
			}
		}
	}
}
*/
void exportMap(){
	std::ofstream levelFile;
	levelFile.open("DeathMountain.level");

	for (int i = 0; i < mapH; i++){
		for (int j = 0; j < mapW; j++){
			levelFile << map[i][j].type << "\n";
			levelFile << map[i][j].worldZ << "\n";
		}
		//levelFile << "\n";
	}
	levelFile.close();
}

void clearMap(){
	for (int i = 0; i < mapH; i++){
		for (int j = 0; j < mapW; j++){
			map[i][j].type = 1;
		}
	}
}

bool selectOptionsButton(int inputX, int inputY){
	if (inputX >= save.x && inputX <= save.x + 32 && inputY >= save.y && inputY <= save.y + 32){
		exportMap();
		printf("save button hit\n");
		return true;
	}
	if (inputX >= trash.x && inputX <= trash.x + 32 && inputY >= trash.y && inputY <= trash.y + 32){
		clearMap();
		printf("trash button hit\n");
		return true;
	}
	if (inputX >= exit_program.x && inputX <= exit_program.x + 32 && inputY >= exit_program.y && inputY <= exit_program.y + 32){
		gRunning = false;
		printf("exit program button hit\n");
		return true;
	}
	return false;
}

bool selectPaletteButton(int inputX, int inputY){
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			if (inputX >= paletteButtons[i][j].x && inputX < paletteButtons[i][j].x + spriteSize){
				if (inputY >= paletteButtons[i][j].y && inputY < paletteButtons[i][j].y + spriteSize){
					currentType = paletteButtons[i][j].tileType;
					return true;
				}
			}
		}
	}
	return false; 
}

void handleInputs(){
	while (SDL_PollEvent(&evnt)){
		switch (evnt.type){
		case SDL_QUIT:
			gRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			if (evnt.button.button == SDL_BUTTON_LEFT){
				if (!selectPaletteButton(mouseX, mouseY)){
					if (!selectOptionsButton(mouseX, mouseY)){
						selectTile(mouseX, mouseY);
					}
				}
			}
			if (evnt.button.button == SDL_BUTTON_RIGHT){
				cycleTileZValue(mouseX, mouseY);
			}
			//selectTile(mouseX, mouseY);
			break;
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym){
			case SDLK_w:
				renderOffsetY += 20;
				break;
			case SDLK_s:
				renderOffsetY -= 20;
				break;
			case SDLK_a:
				renderOffsetX += 20;
				break;
			case SDLK_d:
				renderOffsetX -= 20;
				break;
			default: 
				break;
			}
			break;
		default:
			break;
		}
	}

	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	if(mouseX < 10) 			renderOffsetX += 5;
	if(mouseX > screenW - 10) 	renderOffsetX -= 5;
	if(mouseY < 10)				renderOffsetY += 5;
	if(mouseY > screenH - 10)	renderOffsetY -= 5;
}


int main(int argv, char* argc[]){
	gRunning = initSDL();
	initTextures();
	initPaletteButtons();
	initMap();

	while (gRunning){
		handleInputs();
		renderAll();
	}
	

	SDL_Quit();
	
	
	
	return 0;
}
