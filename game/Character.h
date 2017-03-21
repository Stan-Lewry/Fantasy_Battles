#ifndef CHARACTER_HEADER
#define CHARACTER_HEADER
#include <iostream>
//#include "SDLUtils.h"

enum Profession{KNIGHT, WIZARD, FIGHTER, SPEARMAN, ARCHER};

class Character{
public:
	Character(int _worldX, int _worldY, int _worldZ, int _screenX, int _screenY, int _spriteID, Profession _profession, char* _name);
	int getWorldX();
	void setWorldX(int _worldX);
	int getWorldY();
	void setWorldY(int _worldY);
	int getWorldZ();
	void setWorldZ(int _worldZ);
	int getScreenX();
	void setScreenX(int _screenX);
	int getScreenY();
	void setScreenY(int _screenY);
	int getMoveRange();
	int getAttkRange();
	int getCurrentHP();
	int getMaxHP();
	char* getName();
	int getSpriteID();
	int getMovePoints();
	void setMovePoints(int p);
	int getAttkPoints();
	void setAttkPoints(int p);
	char* getProfession();
	int getAttk();
	int getArmour();
	Profession getProf();


	int getAnimationFrame();
	void setAnimationFrame(int frame);

	bool isIdle();
	void setIdle(bool i);
	
	void reset();

	void moveTo(int _worldX, int _worldY, int worldZ);
	int attack();


	bool clickedOn(int x, int y, int renderOffsetX, int renderOffsetY);

	void doDamage(int dmg);


	void setDead(bool d);
	bool isDead();
private:
	int worldX;
	int worldY;
	int worldZ;
	int screenX;
	int screenY;
	
	
	char* name;
	int animationFrame;
	bool dead;
	bool idle;
	int spriteID;
	bool selected;

	Profession profession;

	int hp;
	int maxHp;




	int movePoints;
	int maxMovePoints;
	int attkPoints;
	int maxAttkPoints;
	
	int moveRange;
	//int maxMoveRange;
	int attkRange;
	//int maxAttkRange;

	int armour;
	int attk;
	


};

#endif
