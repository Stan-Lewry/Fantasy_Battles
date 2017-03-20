#ifndef WORLD_HEADER
#define WORLD_HEADER

#include "SDLUtils.h"
#include <iostream>
#include <fstream>

class World{
public:	
	mapTile map[mapH][mapH];
	//World();
	mapTile* getMap();
	void initMap();
	void initBlankMap();
	bool isTraversable(int x, int y);
	void checkMovementRange(int moveDist, int originX, int originY);
	void checkAttackRange(int attackDist, int originX, int originY);
	std::vector<Point> getPath(int originX, int originY,
				   int destX, int destY, int moveDist);
	void selectTile(int worldX, int worldY);
	mapTile getTile(int clickX, int clickY,
                        int renderOffsetX, int renderOffsetY);
	mapTile getTileWorldCoords(int worldX, int worldY);
	void clearAll();
	void loadMap(char* path);
	
	void setIsoScreenCoords(int x, int y);

private:

	int spriteSize = 64;
	int tileSize = 128;
};


#endif








/*

mapTile wall6;
wall6.typeX = 6 * spriteSize;
wall6.typeY = 1 * spriteSize;
wall6.blocked = true;
wall6.selected = false;
wall6.moveRange = false;
wall6.attackRange = false;

wall6.screenX = (13 - 11) * tileSize / 2;
wall6.screenY = (13 + 11) * tileSize / 4;
wall6.worldX = 13;
wall6.worldY = 11;
map[11][13] = wall6;


*/
