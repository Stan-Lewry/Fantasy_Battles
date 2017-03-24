

#include "World.h"

mapTile* World::getMap(){
	return *map;
}

void World::initBlankMap(){
	mapTile grass;
	grass.typeX = 1 * spriteSize;
	grass.typeY = 0;
	grass.blocked = false;
	grass.selected = false;
	grass.moveRange = false;
	grass.attackRange = false;
	for (int i = 0; i < mapH; i++){
		for (int j = 0; j < mapW; j++){
			grass.screenX = (j - i) * tileSize / 2;
			grass.screenY = (j + i) * tileSize / 4;
			grass.worldX = j;
			grass.worldY = i;

			map[i][j] = grass;
		}
	}
}

void World::setIsoScreenCoords(int x, int y){
	map[y][x].screenX = (x - y) * tileSize / 2;
	map[y][x].screenY = (x + y) * tileSize / 4;
}


void World::loadMap(char* path){
	std::string line;
	std::fstream levelFile(path);

	for(int i = 0; i < mapW * mapH; i++){
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

bool World::isTraversable(int x, int y){
	if (x >= 0){
		if (x < mapW){
			if (y >= 0){
				if (y < mapH){
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

//Look into splitting this up into several smaller function

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


mapTile World::getTile(int clickX, int clickY, int renderOffsetX, int renderOffsetY){


	for (int i = 0; i < mapH; i++){
		for (int j = 0; j < mapW; j++){
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

mapTile World::getTileWorldCoords(int worldX, int worldY){
	return map[worldY][worldX];
}

void World::selectTile(int worldX, int worldY){
	map[worldY][worldX].selected = true;
}

void World::clearAll(){
	for (int i = 0; i < mapH; i++){
		for (int j = 0; j < mapW; j++){
			map[i][j].selected = false;
			map[i][j].moveRange = false;
			map[i][j].attackRange = false;
			map[i][j].occupiedF = false;
			map[i][j].occupiedE = false;
		}
	}
}


void World::setOccupiedTiles(Character* friendlyCharacters[teamSize], Character* enemyCharacters[teamSize]){
	for(int i = 0; i < teamSize; i++){
		map[friendlyCharacters[i]->getWorldY()][friendlyCharacters[i]->getWorldX()].occupiedF = true;
		map[enemyCharacters[i]->getWorldY()][enemyCharacters[i]->getWorldX()].occupiedE = true;


	}
}