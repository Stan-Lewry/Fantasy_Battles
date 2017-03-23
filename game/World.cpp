

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



//look into condensing this needs doing urgently
void World::loadMap(char* path){
	std::string line;
	std::fstream levelFile(path);

	int i = 0;
	int j = 0;
	
	//int lineCount = (mapH * mapW) * 2;
	int counter = 0;
	//for (counter = 0; counter < mapW * mapH)
	//bool reading = true;

	for (counter = 0; counter < mapW * mapH; counter++){
		
		//initialize blank tile
		mapTile newTile;
		newTile.worldX = j;
		newTile.worldY = i;
		newTile.worldZ = 0;
		newTile.screenX = (newTile.worldX - newTile.worldY) * tileSize / 2;
		newTile.screenY = (newTile.worldX + newTile.worldY) * tileSize / 4;
		newTile.selected = false;
		newTile.attackRange = false;
		newTile.moveRange = false;
		newTile.exists = true;
		newTile.occupiedF = false;
		newTile.occupiedE = false;
		
		//get first line - tile type
		std::getline(levelFile, line);

		//assigne tile type
		if (line == "0"){
			newTile.typeX = 0;
			newTile.typeY = 0;
			newTile.blocked = true;

		}
		else if (line == "1"){
			//grass tile 0 is debug tile
			newTile.typeX = 1 * spriteSize;
			newTile.typeY = 0 * spriteSize;
			newTile.blocked = false;

		}
		else if (line == "2"){
			//path tile
			newTile.typeX = 2 * spriteSize;
			newTile.typeY = 0 * spriteSize;
			newTile.blocked = false;

		}
		else if (line == "3"){
			//path tile
			newTile.typeX = 3 * spriteSize;
			newTile.typeY = 0 * spriteSize;
			newTile.blocked = false;

		}
		else if (line == "4"){
			//path tile
			newTile.typeX = 4 * spriteSize;
			newTile.typeY = 0 * spriteSize;
			newTile.blocked = false;

		}
		else if (line == "5"){
			//path tile
			newTile.typeX = 5 * spriteSize;
			newTile.typeY = 0 * spriteSize;
			newTile.blocked = false;

		}
		else if (line == "6"){
			//path tile
			newTile.typeX = 6 * spriteSize;
			newTile.typeY = 0 * spriteSize;
			newTile.blocked = false;

		}
		else if (line == "7"){
			//path tile
			newTile.typeX = 7 * spriteSize;
			newTile.typeY = 0 * spriteSize;
			newTile.blocked = false;

		}
		else if (line == "8"){
			//path tile
			newTile.typeX = 8 * spriteSize;
			newTile.typeY = 0 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "9"){
			//path tile
			newTile.typeX = 9 * spriteSize;
			newTile.typeY = 0 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "10"){
			//path tile
			newTile.typeX = 0 * spriteSize;
			newTile.typeY = 1 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "11"){
			//path tile
			newTile.typeX = 1 * spriteSize;
			newTile.typeY = 1 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "12"){
			//path tile
			newTile.typeX = 2 * spriteSize;
			newTile.typeY = 1 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "13"){
			//path tile
			newTile.typeX = 3 * spriteSize;
			newTile.typeY = 1 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "14"){
			//path tile
			newTile.typeX = 4 * spriteSize;
			newTile.typeY = 1 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "15"){
			//path tile
			newTile.typeX = 5 * spriteSize;
			newTile.typeY = 1 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "16"){
			//path tile
			newTile.typeX = 6 * spriteSize;
			newTile.typeY = 1 * spriteSize;
			newTile.blocked = false;

		}
		else if (line == "20"){
			//path tile
			newTile.typeX = 0 * spriteSize;
			newTile.typeY = 2 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "21"){
			//path tile
			newTile.typeX = 1 * spriteSize;
			newTile.typeY = 2 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "22"){
			//path tile
			newTile.typeX = 2 * spriteSize;
			newTile.typeY = 2 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "23"){
			//path tile
			newTile.typeX = 3 * spriteSize;
			newTile.typeY = 2 * spriteSize;
			newTile.blocked = true;
		}
		else if (line == "24"){
			//path tile
			newTile.typeX = 4 * spriteSize;
			newTile.typeY = 2 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "25"){
			//path tile
			newTile.typeX = 5 * spriteSize;
			newTile.typeY = 2 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "26"){
			//path tile
			newTile.typeX = 6 * spriteSize;
			newTile.typeY = 2 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "27"){
			//path tile
			newTile.typeX = 7 * spriteSize;
			newTile.typeY = 2 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "28"){
			//path tile
			newTile.typeX = 8 * spriteSize;
			newTile.typeY = 2 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "29"){
			//path tile
			newTile.typeX = 9 * spriteSize;
			newTile.typeY = 2 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "30"){
			//path tile
			newTile.typeX = 0 * spriteSize;
			newTile.typeY = 3 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "31"){
			//path tile
			newTile.typeX = 1 * spriteSize;
			newTile.typeY = 3 * spriteSize;
			newTile.blocked = true;

		}
		else if (line == "32"){
			//path tile
			newTile.typeX = 2 * spriteSize;
			newTile.typeY = 3 * spriteSize;
			newTile.blocked = true;

		}
		//get next line - this will be the zAxis 
		std::getline(levelFile, line);
		std::stringstream convert(line);
		convert >> newTile.worldZ;

		//string to int, then assign it to the current tile

		//std::getline(levelFile, currentLine);

		map[i][j] = newTile;
		j++;
		if (j > 14){
			i++;
			j = 0;
		}
	}
	levelFile.close();

}

/*
This function
*/

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
				//if(map[]) if adjacent cells z val is within one of the current z value
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
				//if(currentZ - map[currentY - 1][currentX].worldZ >= -1 && currentZ - map[current])
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

			/*
			if (clickX >= map[i][j].screenX + renderOffsetX && clickX < map[i][j].screenX + renderOffsetX + tileSize){
				if (clickY >= map[i][j].screenY + renderOffsetY && clickY < map[i][j].screenY + renderOffsetY + tileSize){	
					std::cout << "clicked at: " << j << ", " << i << std::endl;
					
					//map[i][j].selected = true;
					
					//checkMovementRange(5, map[i][j].worldX, map[i][j].worldY);
					
					return map[i][j];
					//map[5][4].selected = true;
				}
			}
			*/
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