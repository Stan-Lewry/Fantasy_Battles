// NOTES: REMOVE ALL KNOWLEDGE OF THE GAME RULES -- MIGRATE TO GAME CLASS, 
// CHARACTER SHOULD ONLY STORE VALUES NOT GAME LOGIC

#include "Character.h"
//Constructor
//Switch statement sets up certain variables based on the given prefesion
Character::Character(int _worldX, int _worldY, int _worldZ,int _screenX, int _screenY, int _spriteID, Profession _profession, char* _name) {
	worldX = _worldX;
	worldY = _worldY;
	worldZ = _worldZ;
	screenX = (worldX - worldY) * 128 / 2;
	screenY = (worldX + worldY) * 128 / 4;
	spriteID = _spriteID;
	name = _name;
	selected = false;
	dead = false;
	animationFrame = 2;

	profession = _profession;
	
	switch (profession){
	case KNIGHT:
		maxHp = 100;
		hp = maxHp;
		movePoints = 1;
		maxMovePoints = 1;
		attkPoints = 1;
		maxAttkPoints = 1;
		moveRange = 2;
		attkRange = 1;

		armour = 12;
		attk = 15;
		break;
	case WIZARD:
		maxHp = 70;
		hp = maxHp;
		movePoints = 1;
		maxMovePoints = 1;
		attkPoints = 2;
		maxAttkPoints = 2;
		moveRange = 3;
		attkRange = 5;

		armour = 8;
		attk = 15;
		break;
	case FIGHTER:
		maxHp = 50;
		hp = maxHp;
		movePoints = 2;
		maxMovePoints = 2;
		attkPoints = 1;
		maxAttkPoints = 1;
		moveRange = 5;
		attkRange = 1;

		armour = 6;
		attk = 20;
		break;
	case SPEARMAN:
		maxHp = 50;
		hp = maxHp;
		movePoints = 1;
		maxMovePoints = 1;
		attkPoints = 1;
		maxAttkPoints = 1;
		moveRange = 5;
		attkRange = 2;

		armour = 6;
		attk = 25;
		break;
	case ARCHER:
		maxHp = 70;
		hp = maxHp;
		movePoints = 1;
		maxMovePoints = 1;
		attkPoints = 2;
		maxAttkPoints = 2;
		moveRange = 3;
		attkRange = 5;

		armour = 8;
		attk = 15;
	default:
		break;
	}
}

//Returns worldX
int Character::getWorldX(){
	return worldX;
}

//Sets new worldX
void Character::setWorldX(int _worldX){
	worldX = _worldX;
}

//Retursn worldY
int Character::getWorldY(){
	return worldY;
}

//Sets new worldY
void Character::setWorldY(int _worldY){
	worldY = _worldY;
}

//Returns worldZ
int Character::getWorldZ(){
	return worldZ;
}

//Sets new worldZ
void Character::setWorldZ(int _worldZ){
	worldZ = _worldZ;
}

//Returns screenX
int Character::getScreenX(){
	return screenX;
}

//Sets new screenX
void Character::setScreenX(int _screenX){
	screenX = _screenX;
}

//Returns screenY
int Character::getScreenY(){
	return screenY;
}

//Sets new screenY
void Character::setScreenY(int _screenY){
	screenY = _screenY;
}

//Returns moveRange
int Character::getMoveRange(){
	return moveRange;
}

//Returns attackRange
int Character::getAttkRange(){
	return attkRange;
}

//Returns the current HP
int Character::getCurrentHP(){
	return hp;
}

//Returns the maximum HP
int Character::getMaxHP(){
	return maxHp;
}

//Returns the characters name
char* Character::getName(){
	return name;
}

//Returns the spriteID
int Character::getSpriteID(){
	return spriteID;
}

//Returns the current number of move points
int Character::getMovePoints(){
	return movePoints;
}

//sets the move points - add p to the current move points
void Character::setMovePoints(int p){
	movePoints += p;
}

//returns the current attack points
int Character::getAttkPoints(){
	return attkPoints;
}

//Sets the attack points - add p to current attack point
void Character::setAttkPoints(int p){
	attkPoints += p;
}

//Returns animationFrame
int Character::getAnimationFrame(){
	return animationFrame;
}

//Sets the animation frame
void Character::setAnimationFrame(int frame){
	animationFrame = frame;
}

//Returns attk
int Character::getAttk(){
	return attk;
}

//returns armour
int Character::getArmour(){
	return armour;
}


//Returns the characters profession as a c-style string
char* Character::getProfession(){
	switch (profession){
	case KNIGHT: return "knight"; break;
	case WIZARD: return "wizard"; break;
	case FIGHTER: return "fighter"; break;
	default: break;
	}
}

//Returns the porfession
Profession Character::getProf(){
	return profession;
}

//returns idle
bool Character::isIdle(){
	return idle;
}

//sets idle. Changes the animation frame. if i is true it returns move points and attack points to 1
//This needs to be changed, Character should not contain game logic
void Character::setIdle(bool i){
	if (i == true){
		idle = true;
		animationFrame = 1;
	}
	else{
		movePoints = 1;
		attkPoints = 1;
		idle = false;
		animationFrame = 2;
	}
}

//Resets all values to their max
//Perhaps remove this too? does this count as game logic?
void Character::reset(){
	movePoints = maxMovePoints;
	attkPoints = maxAttkPoints;
	
	if(!dead) animationFrame = 2;

	idle = false;
}

//Sets dead and updates animation frame appropriatly
void Character::setDead(bool d){
	dead = d;
	animationFrame = 0;
}

//Returns dead
bool Character::isDead(){
	return dead;
}

//checks if the given x,y values are on the character. The hitbox is slightly smaller
//that the actual sprite
bool Character::clickedOn(int x, int y, int rendererOffsetX, int rendererOffsetY){
	if (x >= screenX + rendererOffsetX + 32 && x < screenX + rendererOffsetX + 96){
		if (y >= screenY + rendererOffsetY && y < screenY + rendererOffsetY + 96){
			printf("clicked on character\n");
			return true;
		}
		else return false;
	}
	else return false;
}

//Moves the character to the give coords
void Character::moveTo(int _worldX, int _worldY, int _worldZ){

	std::cout << "moving character to" << _worldX << "," << _worldY << std::endl;
	worldX = _worldX;
	worldY = _worldY;
	worldZ = _worldZ;
	screenX = (worldX - worldY) * 128 / 2;
	screenY = (worldX + worldY) * 128 / 4;
	movePoints -= 1;
	//attkPoints -= 1; //TEMPORARY _ FOR TESTING PURPOSES ONLY
}

//returns attk - needs to be reconsidered
int Character::attack(){
	attkPoints -= 1;
	return attk;
}

//applies damage to the character. Sets dead to true if appropriate needs reconsidering
void Character::doDamage(int dmg){
	if (hp - dmg <= 0){
		hp = 0;
		dead = true;
		animationFrame = 0;
	}
	else{
		hp -= dmg;
	}
}

//assigns the new path to the characters path, path is a vector of points - (x,y pairs)
void Character::assignPath(std::vector<Point> newPath){
	path = newPath;
}

//calls animateToTile on the first Point in the path, if it returns true it throws out the first element
//if path is not greater then 0 elements it does nothing and sets isMoving to false;
void Character::animateAlongPath(){
	if(path.size() > 0){
		isMoving = true;

		if(path.at(0).x > worldX){											// check the direction of the next cell
			std::cout << "moving right\n";
			//moveTo(path.at(0).x, path.at(0).y, 0);
			//path.erase(path.begin() + 0);									// erase the first element from the path
		}
		else if(path.at(0).x < worldX){
			std::cout << "moving left\n";
			//moveTo(path.at(0).x, path.at(0).y, 0);
			//path.erase(path.begin() + 0);
		}
		else if(path.at(0).y > worldY){
			std::cout << "moving down\n";
			//moveTo(path.at(0).x, path.at(0).y, 0);
			//path.erase(path.begin() + 0);
		}
		else if(path.at(0).y < worldY){
			std::cout << "moving left\n";
			//moveTo(path.at(0).x, path.at(0).y, 0);
			//path.erase(path.begin() + 0);
		}

		bool arrived = animateToTile(path.at(0).x, path.at(0).y);
		if(arrived){
			path.erase(path.begin() + 0);
		}

	}else{
		isMoving = false;
	}

}

//Takes a set of world coords. Checks where the new coords are in relation to
//the characters current position and moves (in screen values) towards that position
//if the character arrives it returns true, else it returns false
bool Character::animateToTile(int tileWorldX, int tileWorldY){
	int screenDestX = (tileWorldX - tileWorldY) * 128 / 2;
	int screenDestY = (tileWorldX + tileWorldY) * 128 / 4;

	if(screenDestY == screenY && screenDestX == screenX){

		std::cout << "RETURNING TRUE\n";
		worldX = tileWorldX;
		worldY = tileWorldY;
		//worldZ = ???;
		return true;
	}

	else if(tileWorldX > worldX){
		//std::cout << "moving right\n";
		screenX += 2;
		screenY += 1;
		return false;
		//moving to the right
	}
	else if(tileWorldX < worldX){
		//std::cout << "moving left\n";
		screenX -= 2;
		screenY -= 1;
		return false;
		//moving to the left
	}else if(tileWorldY > worldY){
		//std::cout << "moving down\n";
		screenY += 1;
		screenX -= 2;
		return false;
		//moving down
	}
	else if(tileWorldY < worldY){
		//std::cout << "moving up\n";
		screenX += 2;
		screenY -= 1;
		return false;
		//moving up
	}

}