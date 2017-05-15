// World Class
// This class primarlly handles the map and all associated functionality
// at the core of this class is a 2d array of mapTiles
// This class also deals with pathfinding and range finding functionality
// Details of implemetation can be found in World.cpp kdjfkjdskjfkjdkf

#ifndef WORLD_HEADER
#define WORLD_HEADER

#include "SDLUtils.h"
#include "Character.h"
#include <iostream>
#include <fstream>

class World{
public:	
	mapTile map[mapH][mapW];
	//World();

	//Map* getTheMap();
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

	void setOccupiedTiles(Character* friendlyCharacters[teamSize], Character* enemyCharacters[teamSize]);
	void clearAll();
	void loadMap(char* path);
	
	void setIsoScreenCoords(int x, int y);

	int getMapWidth();
	int getMapHeight();

	Point* getBlueSpawners();
	Point* getRedSpawners();

private:

	int spriteSize = 64;
	int tileSize = 128;
	int mapWidth;
	int mapHeight;

	Point blueSpawners[5];
	Point redSpawners[5];
};


#endif

