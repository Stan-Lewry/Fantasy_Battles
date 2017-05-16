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

const int mapMaxWidth = 50;
const int mapMaxHeight = 50;

int mapWidth = 33;
int mapHeight = 10;

SDL_Window* window;
SDL_Renderer* rend;
SDL_Texture* tiles;
SDL_Texture* ui;
SDL_Texture* paletteSheet;
SDL_Event evnt;
TTF_Font* font = NULL; 

int renderOffsetX = 300; 
int renderOffsetY = 0;

int paletteX = screenW - 340;
int paletteY = 35;
//int palettespriteSize = 32;

int currentType = 1;

bool gRunning = false;

struct tileInfo{
	int typeX;
	int typeY;
	bool blocked;
	bool blueSpawner;
	bool redSpawner;
};

struct tile{
	int worldX;
	int worldY;
	int worldZ;
	int screenX;
	int screenY;
	tileInfo info;
};

struct paletteButton{
	int x;
	int y;
	int w;
	int h;
	int tileType;
	tileInfo info;
};

struct Button{
	int x, y, w, h;
};


Button trash = { screenW - 108 - 32, screenH - 55 - 32, 32, 32 };
Button save = { 1076, screenH - 55 - 32, 32, 32 };
Button exit_program = { 1012, screenH - 55 - 32, 32, 32 };

tile map[mapMaxHeight][mapMaxWidth];
tileInfo brush = {1, 0, false, false, false};


paletteButton paletteButtons[10][10]; 

bool initSDL(){
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	font = TTF_OpenFont("Fonts/half_bold_pixel.ttf", 30);
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
	tiles = loadPNG("Assets/iso_tiles_large_spawners01.png");
	paletteSheet = loadPNG("Assets/palette02.png");
	ui = loadPNG("Assets/UI01.png");
}

void initMap(){
	for (int i = 0; i < mapMaxHeight; i++){
		for (int j = 0; j < mapMaxWidth; j++){
			tileInfo newTileInfo = {1, 0, false, false, false};
			tile newTile = { j, i, 0, j * tileSize, i * tileSize, newTileInfo};
	

			map[i][j] = newTile;
		}
	}
}

void initPaletteButtons(){
	int type = 0;
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			tileInfo newInfo = {j, i, false, false, false};
			paletteButton pb = { j * spriteSize + paletteX, i * spriteSize + paletteY, spriteSize, spriteSize, type , newInfo};
			paletteButtons[i][j] = pb;
			type++; 
		}
	}
	
	paletteButtons[0][8].info.blocked = true;
	paletteButtons[0][9].info.blocked = true;
	paletteButtons[1][1].info.blocked = true;
	paletteButtons[1][2].info.blocked = true;
	paletteButtons[1][3].info.blocked = true;
	paletteButtons[1][4].info.blocked = true;
	paletteButtons[1][5].info.blocked = true;
	paletteButtons[1][6].info.blocked = true;
	paletteButtons[2][0].info.blocked = true;
	paletteButtons[2][1].info.blocked = true;
	paletteButtons[2][2].info.blocked = true;
	paletteButtons[2][3].info.blocked = true;
	paletteButtons[2][4].info.blocked = true;
	paletteButtons[2][5].info.blocked = true;
	paletteButtons[2][6].info.blocked = true;
	paletteButtons[2][7].info.blocked = true;
	paletteButtons[2][8].info.blocked = true;
	paletteButtons[2][9].info.blocked = true;
	paletteButtons[3][0].info.blocked = true;
	paletteButtons[3][1].info.blocked = true;
	paletteButtons[3][2].info.blocked = true;
	paletteButtons[3][4].info.blocked = true;
	paletteButtons[3][5].info.blocked = true;
	paletteButtons[3][6].info.blocked = true;
	paletteButtons[3][7].info.blocked = true;
	paletteButtons[3][8].info.blocked = true;
	paletteButtons[3][9].info.blocked = true;
	paletteButtons[4][0].info.blocked = true;
	paletteButtons[4][1].info.blocked = true;
	paletteButtons[4][2].info.blocked = true;
	paletteButtons[4][3].info.blocked = true;
	paletteButtons[4][4].info.blocked = true;
	paletteButtons[4][5].info.blocked = true;

	paletteButtons[1][8].info.blueSpawner = true;
	paletteButtons[1][9].info.redSpawner = true;
}

void renderSelectedTile(){
	SDL_Rect sRect = { 0, 0, spriteSize, spriteSize };
	SDL_Rect dRect = { screenW - 230, screenH - 295, 100, 100 };
	sRect.x = brush.typeX * spriteSize;
	sRect.y = brush.typeY * spriteSize;
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

void renderText(const char* text, int screenX, int screenY){
	SDL_Rect dRect = {screenX, screenY, 0, 0};
	TTF_SizeText(font, text, &dRect.w, &dRect.h);
	SDL_Color col = {255, 255, 255};
	SDL_Surface* tempSurf = TTF_RenderText_Solid(font, text, col);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, tempSurf);
	SDL_RenderCopy(rend, texture, NULL, &dRect);
	SDL_FreeSurface(tempSurf);
	SDL_DestroyTexture(texture);
}

void renderMap(){
	SDL_Rect bg = { 0, 0, screenW, screenH };
	SDL_SetRenderDrawColor(rend, 102, 204, 255, 1);
	SDL_RenderFillRect(rend, &bg);

	SDL_Rect sRect = { 0, 0, worldSpriteSize, worldSpriteSize };
	SDL_Rect dRect = { 0, 0, screenTileSize, screenTileSize };


	for (int i = 0; i < mapHeight; i++){
		for (int j = 0; j < mapWidth; j++){

			sRect.x = map[i][j].info.typeX;
			sRect.y = map[i][j].info.typeY;

			sRect.x *= worldSpriteSize;
			sRect.y *= worldSpriteSize;

			dRect.x = (map[i][j].screenX - map[i][j].screenY) / 2;
			dRect.y = (map[i][j].screenX + map[i][j].screenY) / 4;

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

			dRect.y -= height;
			SDL_RenderCopy(rend, tiles, &sRect, &dRect);

			if(map[i][j].info.blueSpawner){
				sRect.x = 8 * worldSpriteSize;
				sRect.y = 1 * worldSpriteSize;
				SDL_RenderCopy(rend, tiles, &sRect, &dRect);
			}
			if(map[i][j].info.redSpawner){
				sRect.x = 9 * worldSpriteSize;
				sRect.y = 1 * worldSpriteSize;
				SDL_RenderCopy(rend, tiles, &sRect, &dRect);
			}
			
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

	//renderText(std::to_string(mapWidth))
	std::string mapSizeString = std::to_string(mapWidth).append("x").append(std::to_string(mapHeight));

	renderText(mapSizeString.c_str(), 50, screenH - 85);

	/*
	SDL_Rect xUpArrow = {38, screenH - 125, 32, 32};
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 1);
	SDL_RenderFillRect(rend, &xUpArrow);

	SDL_Rect yUpArrow = {133, screenH - 125, 32, 32};
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 1);
	SDL_RenderFillRect(rend, &yUpArrow);

	SDL_Rect xDownArrow = {38, screenH - 50, 32, 32};
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 1);
	SDL_RenderFillRect(rend, &xDownArrow);

	SDL_Rect yDownArrow = {133, screenH - 50, 32, 32};
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 1);
	SDL_RenderFillRect(rend, &yDownArrow);
	*/

}

void renderAll(){
	SDL_RenderClear(rend);
	renderMap();
	renderUI();
	renderSelectedTile();
	SDL_RenderPresent(rend);
}



void selectTile(int inputX, int inputY){
	for (int i = 0; i < mapHeight; i++){
		for (int j = 0; j < mapWidth; j++){

			int tileHeight = map[i][j].worldZ * 16;

			int minX = (map[i][j].screenX - map[i][j].screenY) / 2;
			minX += renderOffsetX;
			minX += 32;
			minX += 32/4;

			int minY = (map[i][j].screenX + map[i][j].screenY) / 4;
			minY += renderOffsetY;
			minY += 64;
			minX += (32 / 8) * 3;
			minY -= tileHeight;

			int maxX = minX + 32;
			int maxY = minY + 16;

			if(inputX >= minX && inputX < maxX){
				if(inputY >= minY && inputY < maxY){
					if(brush.blueSpawner){
						map[i][j].info.blueSpawner = true;
					}
					else if(brush.redSpawner){
						map[i][j].info.redSpawner = true;
					}
					else map[i][j].info = brush;
				}
			}
		}
	}
}

bool cycleTileZValue(int inputX, int inputY){
	for (int i = mapHeight; i > 0; i--){
		for (int j = mapWidth; j > 0; j--){

			int tileHeight = map[i][j].worldZ * 16;
			int minX = (map[i][j].screenX - map[i][j].screenY) / 2;
			minX += renderOffsetX;
			minX += 32;
			minX += 32/4;

			int minY = (map[i][j].screenX + map[i][j].screenY) / 4;
			minY += renderOffsetY;
			minY += 64;
			minX += (32 / 8) * 3;
			minY -= tileHeight;

			int maxX = minX + 32;
			int maxY = minY + 16;

			if(inputX >= minX && inputX < maxX){
				if(inputY >= minY && inputY < maxY){
					if(map[i][j].worldZ < 10){
						map[i][j].worldZ += 1;
					}
					else {
						map[i][j].worldZ = 0;
					}

					return true;
				}
			}
		}
	}
	return false;
}


//this now needs to include writing out the spawners and the map size
//also include writing out a background colour
void exportMap(){
	std::ofstream levelFile;
	levelFile.open("plane.level");

	//First thing we want to do is write out the width and height of the map 
	levelFile << mapWidth << "\n";							//Write the width of the map
	levelFile << mapHeight << "\n";							//Write the height of the map
	levelFile << "255\n";									//Background R
	levelFile << "255\n";									//Background G
	levelFile << "255\n";									//Background B

	//We then construct a list of blue and red team spawners in the map which will be written next
	tile blueSpawners[5];
	tile redSpawners[5];
	int redSpawnersFound = 0;
	int blueSpawnersFoud = 0;
	for(int i = 0; i < mapHeight; i++){
		for(int j = 0; j < mapWidth; j++){
			if(map[i][j].info.blueSpawner && blueSpawnersFoud < 5){
				blueSpawners[blueSpawnersFoud] = map[i][j];
				blueSpawnersFoud += 1;
			}
			if(map[i][j].info.redSpawner && redSpawnersFound < 5){
				redSpawners[redSpawnersFound] = map[i][j];
				redSpawnersFound += 1;
			}
		}
	}

	//Now we write the world coords of these spawners to our file
	//blue team first
	for(int i = 0; i < 5; i++){
		levelFile << blueSpawners[i].worldX << std::endl;
		levelFile << blueSpawners[i].worldY << std::endl;
	}
	//then the red team
	for(int i = 0; i < 5; i ++){
		levelFile << redSpawners[i].worldX << "\n";
		levelFile << redSpawners[i].worldY << "\n";
	}

	for(int i = 0; i < mapHeight; i++){
		for(int j = 0; j < mapWidth; j++){
			levelFile << map[i][j].worldX << "\n";			//Write the world X
			levelFile << map[i][j].worldY << "\n";			//Write the world y
			levelFile << map[i][j].worldZ << "\n";			//Write the world z
			levelFile << map[i][j].info.typeX << "\n";		//Write the spritesheet x
			levelFile << map[i][j].info.typeY << "\n";		//Write the spritesheet y
			if(map[i][j].info.blocked) levelFile << "1\n";	//1 = blocked is true
			else levelFile << "0\n"; 						//0 = blocked is false
		}
	}
}

void clearMap(){
	for (int i = 0; i < mapMaxHeight; i++){
		for (int j = 0; j < mapMaxWidth; j++){
			map[i][j].info = {1, 0, false};
			map[i][j].worldZ = 0;
		}
	}
}

bool selectMapScaleButtons(int inputX, int inputY){
	/*
	SDL_Rect xUpArrow = {38, screenH - 125, 32, 32};
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 1);
	SDL_RenderFillRect(rend, &xUpArrow);

	SDL_Rect yUpArrow = {133, screenH - 125, 32, 32};
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 1);
	SDL_RenderFillRect(rend, &yUpArrow);

	SDL_Rect xDownArrow = {38, screenH - 50, 32, 32};
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 1);
	SDL_RenderFillRect(rend, &xDownArrow);

	SDL_Rect yDownArrow = {133, screenH - 50, 32, 32};
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 1);
	SDL_RenderFillRect(rend, &yDownArrow);
	*/


	if(inputX >= 38 && inputX < 38 + 32  && inputY >= screenH - 125 && inputY < screenH - 125 + 32){
		//x up arrow
		std::cout << "x up arrow clicked\n";
		if(mapWidth < mapMaxWidth){
			mapWidth += 1;
		}
	}
	if(inputX >= 133 && inputX < 133 + 32 && inputY >= screenH - 125 && inputY < screenH - 125 + 32){
		//y up arrow
		std::cout << "y up arrow clicked\n";
		if(mapHeight < mapMaxHeight){
			mapHeight += 1;
		}
	}
	if(inputX >= 38 && inputX < 38 + 32 && inputY >= screenH - 50 && inputY < screenH - 50 + 32){
		//x down arrow
		std::cout << "x down arrow clicked\n";
		if(mapWidth > 1){
			mapWidth -= 1;
		}
	}
	if(inputX >= 133 && inputX < 133 + 32 && inputY >= screenH - 50 && inputY < screenH - 50 + 32){
		//y down arrow
		std::cout << "y down arrow clicked\n";
		if(mapHeight > 1){
			mapHeight -= 1;
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
					//currentType = paletteButtons[i][j].tileType;
					brush = paletteButtons[i][j].info;
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
						if(!selectMapScaleButtons(mouseX, mouseY)){
							selectTile(mouseX, mouseY);
						}
					}
				}
			}
			if (evnt.button.button == SDL_BUTTON_RIGHT){
				//cycleTileZValue(mouseX, mouseY);
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
