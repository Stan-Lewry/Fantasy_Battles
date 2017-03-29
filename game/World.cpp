#include "World.h"


// mapTile* World::getMap()
// returns the entire map
mapTile* World::getMap(){
	return *map;

}

// void World::initBlankMap()
// initialises a blank map - currently not used anyware
void World::initBlankMap(){
	mapTile grass;
	grass.typeX = 1 * spriteSize;
	grass.typeY = 0;
	grass.blocked = false;
	grass.selected = false;
	grass.moveRange = false;
	grass.attackRange = false;
	for (int i = 0; i < mapHeight; i++){
		for (int j = 0; j < mapWidth; j++){
			grass.screenX = (j - i) * tileSize / 2;
			grass.screenY = (j + i) * tileSize / 4;
			grass.worldX = j;
			grass.worldY = i;

			map[i][j] = grass;
		}
	}
}

// void World::setIsoScreenCoords(int x, int y)
// takes a world x and y coords and sets the calculates new values for the tiles screen coords used for the isometric projection
// currently not used anyware
void World::setIsoScreenCoords(int x, int y){
	map[y][x].screenX = (x - y) * tileSize / 2;
	map[y][x].screenY = (x + y) * tileSize / 4;
}

// void World::loadMap(char* path)
// Takes a path to a map
// initialises the map and auxiliary values by reading the input file line by line
// current format for .level file is:
// Line 0 											- Map Width
// Line 1 											- Map Height
// Lines 2 to 4 									- background R, G, B respectivly - currently ignored
// Lines 5 to 14 									- Spawner coords for blue team spawners (in format x then y (world space coords))
// Lines 15 to 24 									- Spawner coords for red team spawners (in format x then y (world space coords))
// Lines 24 to 24 + (screenWidth * screenHeight)	- Tile info for each tile in the world
void World::loadMap(char* path){
	std::string line;
	std::fstream levelFile(path);

	std::getline(levelFile, line);
	mapWidth = std::stoi(line);												// read map width

	std::getline(levelFile, line);
	mapHeight = std::stoi(line);											// read map height

	// next three lines are background rgb values

	std::getline(levelFile, line);
	std::getline(levelFile, line);
	std::getline(levelFile, line);

	//Build the list of blue team spawners
	for(int i = 0; i < 5; i++){
		std::getline(levelFile, line);
		blueSpawners[i].x = std::stoi(line);
		std::getline(levelFile, line);
		blueSpawners[i].y = std::stoi(line);
	}

	//Build the list of red team spawners
	for(int i = 0; i < 5; i ++){
		std::getline(levelFile, line);
		redSpawners[i].x = std::stoi(line);
		std::getline(levelFile, line);
		redSpawners[i].y = std::stoi(line);
	}

	// now do the same as before

	for(int i = 0; i < mapWidth * mapHeight; i++){
		mapTile newTile;

		std::getline(levelFile, line);
		newTile.worldX = std::stoi(line);
		std::getline(levelFile, line);
		newTile.worldY = std::stoi(line);
		std::getline(levelFile, line);
		newTile.worldZ = std::stoi(line);

		newTile.screenX = (newTile.worldX - newTile.worldY) * tileSize / 2;
		newTile.screenY = (newTile.worldX + newTile.worldY) * tileSize / 4;

		std::getline(levelFile, line);
		newTile.typeX = std::stoi(line) * spriteSize;
		std::getline(levelFile, line);
		newTile.typeY = std::stoi(line) * spriteSize;

		std::getline(levelFile, line);
		if(line == "1") newTile.blocked = true;
		else newTile.blocked = false;

		newTile.selected = false;
		newTile.moveRange = false;
		newTile.exists = true;
		newTile.occupiedF = false;
		newTile.occupiedE = false;

		map[newTile.worldY][newTile.worldX] = newTile;
	}
	levelFile.close();
}

// bool World::isTraversable(int x, int y)
// checks if the tile at x and y is blocked or not
bool World::isTraversable(int x, int y){
	if (x >= 0){
		if (x < mapWidth){
			if (y >= 0){
				if (y < mapHeight){
					if (map[y][x].blocked == false){
						return true;
					}
					else return false;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}
	else return false;
}

// void World::checkMovementRange(int moveDist, int originX, int originY)
// runs a breadth first search across the map from the origin point to the move distance
// this uncovers tiles that are moveable from the origin point
// toggles the moveRange boold for all traversable tiles discovered
void World::checkMovementRange(int moveDist, int originX, int originY){
	std::vector<mapTile> listA;
	std::vector<mapTile> listB;

	listA.push_back(map[originY][originX]);

	std::cout << "origin: " << originX << ", " << originY << std::endl;

	for (int i = 0; i < moveDist; i++){
		for (int j = 0; j < listA.size(); j++){

			int currentX = listA.at(j).worldX;
			int currentY = listA.at(j).worldY;
			int currentZ = listA.at(j).worldZ;

			if (isTraversable(currentX, currentY + 1)){
				if(currentZ - map[currentY + 1][currentX].worldZ >= -1 && currentZ - map[currentY + 1][currentX].worldZ <= 1){
					mapTile southAdj = map[currentY + 1][currentX];
					listB.push_back(southAdj);
					map[southAdj.worldY][southAdj.worldX].moveRange = true;
				}

			}

			if (isTraversable(currentX + 1, currentY)){
				mapTile eastAdj = map[currentY][currentX + 1];
				listB.push_back(eastAdj);
				map[eastAdj.worldY][eastAdj.worldX].moveRange = true;
			}

			if (isTraversable(currentX, currentY - 1)){
				int heightDif = currentZ - map[currentY-1][currentX].worldZ;
				if(heightDif >= -1 && heightDif <= 1){
					mapTile northAdj = map[currentY - 1][currentX];
					listB.push_back(northAdj);
					map[northAdj.worldY][northAdj.worldX].moveRange = true;
				}

			}

			if (isTraversable(currentX - 1, currentY)){
				mapTile westAdj = map[currentY][currentX - 1];
				listB.push_back(westAdj);
				map[westAdj.worldY][westAdj.worldX].moveRange = true;
			}
		}
		listA = listB;
		listB.clear();
	}
	map[originY][originX].moveRange = false;
}

// void World::checkAttackRange(int attackDist, int originX, int originY)
// Sames checkMovementRange except for a characters attack range (Breadth first search)
void World::checkAttackRange(int attackDist, int originX, int originY){
	std::vector<mapTile> listA;
	std::vector<mapTile> listB;

	listA.push_back(map[originY][originX]);

	std::cout << "origin: " << originX << ", " << originY << std::endl;

	for (int i = 0; i < attackDist; i++){
		for (int j = 0; j < listA.size(); j++){

			int currentX = listA.at(j).worldX;
			int currentY = listA.at(j).worldY;

			if (isTraversable(currentX, currentY + 1)){
				mapTile southAdj = map[currentY + 1][currentX];
				listB.push_back(southAdj);
				map[southAdj.worldY][southAdj.worldX].attackRange = true;
			}

			if (isTraversable(currentX + 1, currentY)){
				mapTile eastAdj = map[currentY][currentX + 1];
				listB.push_back(eastAdj);
				map[eastAdj.worldY][eastAdj.worldX].attackRange = true;
			}

			if (isTraversable(currentX, currentY - 1)){
				mapTile northAdj = map[currentY - 1][currentX];
				listB.push_back(northAdj);
				map[northAdj.worldY][northAdj.worldX].attackRange = true;
			}

			if (isTraversable(currentX - 1, currentY)){
				mapTile westAdj = map[currentY][currentX - 1];
				listB.push_back(westAdj);
				map[westAdj.worldY][westAdj.worldX].attackRange = true;
			}
		}
		listA = listB;
		listB.clear();
	}

	map[originY][originX].attackRange = false;
}

// std::vector<Point> World::getPath(int originX, int originY, int destX, int destY, int moveDist)
// Takes and origin x and y, and a destination x and y, and a movement range
// This funciton is a modified breadth first search with early exits
// This function finds the best path from the origin to the destination within the movement range
// The function returns an array of Points (x,y pairs) 
// The function constructs Nodes from tiles in the map. Each node has an x,y and the index of the node in the closed/ visited set that it was discovered from
// At the end of the function the closed set is recursed through using each nodes found from index and the vector of Points is constructed and returned
// in another life i would like to replace this with an implementation of A*
std::vector<Point> World::getPath(int originX, int originY, int destX, int destY, int moveDist){
	std::vector<Node> frontier;
	std::vector<Node> newFrontier;
	std::vector<Node> visited;

	Node originNode = {originX, originY, 0};
	frontier.push_back(originNode);

	int depth = 0;

	for(int i = 0; i < moveDist; i++){ //only search as deep as the movement range. i can be used as a depth measure
		for(int j = 0; j < frontier.size(); j++) {//for each element in the frontier
			int curX = frontier.at(j).x;
			int curY = frontier.at(j).y;
			//check north adjacency
			if(map[curY - 1][curX].moveRange){
				//check if the dest has been found
				if(curY - 1 == destY && curX == destX){
					visited.push_back(frontier.at(j));
					Node newNode = {curX, curY - 1, visited.size() - 1};
					visited.push_back(newNode);
					depth = i + 1;
					goto endloop;
					//add the previous node to visited
					//add the found node to visited
					//break out of loop
				}
				else{
					Node newNode = {curX, curY - 1, (visited.size()) + j}; // i plus j is the depth measure of the parent node
					newFrontier.push_back(newNode);
				}
				//if it has not been found add this node to the new frontier
				//i + j is the index of the parent node in visited
			}
			//check the east adjacency
			if(map[curY][curX + 1].moveRange){
				//check if the dest has been found
				if(curY == destY && curX + 1 == destX){
					visited.push_back(frontier.at(j));
					Node newNode = {curX + 1, curY, visited.size() - 1};
					visited.push_back(newNode);
					depth = i + 1;
					goto endloop;
				}
				else{
					Node newNode = {curX + 1, curY, (visited.size()) + j};
					newFrontier.push_back(newNode);
				}
			}
			//check south adjacnecy
			if(map[curY + 1][curX].moveRange){
				//check if the dest has been found
				if(curY + 1 == destY && curX == destX){
					visited.push_back(frontier.at(j));
					Node newNode = {curX, curY + 1, visited.size() - 1};
					visited.push_back(newNode);
					depth = i + 1;
					goto endloop;
				}else{
					Node newNode = {curX, curY + 1, (visited.size()) + j};
					newFrontier.push_back(newNode);
				}
			}
			//check west adjacency
			if(map[curY][curX - 1].moveRange){
				//check if the dest had been found
				if(curY == destY && curX - 1 == destX){
					visited.push_back(frontier.at(j));
					Node newNode = {curX - 1, curY, visited.size() - 1 };
					visited.push_back(newNode);
					depth = i + 1;
					goto endloop;
				}else{
					Node newNode = {curX - 1, curY, (visited.size()) + j};
					newFrontier.push_back(newNode);
				}
			}
		}
		visited.insert(visited.end(), frontier.begin(), frontier.end());
		frontier = newFrontier;
		newFrontier.clear();
		// once every node in the frontier has been searched
		// put the entire frontier into visited
		// put the new frontier into frontier
		// clear the new frontier
	}

	endloop:
	std::vector<Point> path;
	Node currentNode = visited.at(visited.size() - 1);
	for(int i = 0; i < depth; i++){
		Point p = {currentNode.x, currentNode.y};
		path.push_back(p);
		currentNode = visited.at(currentNode.parentIndex);
	}

	std::reverse(path.begin(), path.end());

	return path;
}

// mapTile World::getTile(int clickX, int clickY, int renderOffsetX, int renderOffsetY)
// takes mouse x,y and render offset x,y to test if the input falls within a map tile
// needs render offset values because position onscree != tiles screen coords always
// Because of the way the isometric projection works a small rectangular hitbox is constructed in the center of each tile
// this prevents tile hitboxes from overlapping, but the smaller (than they appear on screen) hitboxes make mouse input a tad fiddly
// this should probably run front to back instead of back to front to accomodate different heights
mapTile World::getTile(int clickX, int clickY, int renderOffsetX, int renderOffsetY){
	for (int i = 0; i < mapHeight; i++){
		for (int j = 0; j < mapWidth; j++){
			//map[i][j].selected = false;

			int minCollisionX = map[i][j].screenX + (tileSize / 4);
			int minCollisionY = map[i][j].screenY + ((tileSize / 8) * 3) - (map[i][j].worldZ * 32);

			int maxCollisionX = minCollisionX + (tileSize / 2);
			int maxCollisionY = minCollisionY + (tileSize / 4) ;

			if (clickX >= minCollisionX + renderOffsetX && clickX < maxCollisionX + renderOffsetX){
				if (clickY >= minCollisionY + renderOffsetY && clickY < maxCollisionY + renderOffsetY){
					std::cout << "Click registered at" << j << "," << i << std::endl;
					return map[i][j];
				}
			}
		}
	}
	mapTile nullTile;
	nullTile.exists = false;
	return nullTile;
}

// mapTile World::getTileWorldCoords(int worldX, int worldY)
// returns the map tile at the given world coords
mapTile World::getTileWorldCoords(int worldX, int worldY){
	return map[worldY][worldX];
}

// void World::selectTile(int worldX, int worldY)
// toggles the select bool of a given tile
void World::selectTile(int worldX, int worldY){
	map[worldY][worldX].selected = true;
}

// void World::clearAll()
// sets all of the helper bools for each tile to false
void World::clearAll(){
	for (int i = 0; i < mapHeight; i++){
		for (int j = 0; j < mapWidth; j++){
			map[i][j].selected = false;
			map[i][j].moveRange = false;
			map[i][j].attackRange = false;
			map[i][j].occupiedF = false;
			map[i][j].occupiedE = false;
		}
	}
}

// void World::setOccupiedTiles(Character* friendlyCharacters[teamSize], Character* enemyCharacters[teamSize])
// specifies the tiles occupied by each character in the lists - this info is used for renderer/preventing characters from entering eachothers tiles
void World::setOccupiedTiles(Character* friendlyCharacters[teamSize], Character* enemyCharacters[teamSize]){
	for(int i = 0; i < teamSize; i++){
		map[friendlyCharacters[i]->getWorldY()][friendlyCharacters[i]->getWorldX()].occupiedF = true;
		map[enemyCharacters[i]->getWorldY()][enemyCharacters[i]->getWorldX()].occupiedE = true;


	}
}

// returns map width
int World::getMapWidth(){
	return mapWidth;
}

// returns mapheight
int World::getMapHeight(){
	return mapHeight;
}

// returns the list of blue spawners
Point* World::getBlueSpawners(){
	return blueSpawners;
}

// returns the list of red spawners
Point* World::getRedSpawners(){
	return redSpawners;
}