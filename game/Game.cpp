#include "Game.h"


// Game::Game()
// Constructor for Game class. 
// Deals with the majority of initialisation including initialising all SDL utilities.
Game::Game(){
	sdlUtils = new SDLUtils();
	input = new Input();
	globalRunning = sdlUtils->initSDL();
	world = new World();
	//world->initBlankMap();
	//world->initMap();
	world->loadMap("Levels/newTestLevel02.level");
	renderer = new Renderer(sdlUtils->rend);
	ui = new UI();
	ui->initMainMenuElements();
	initCharacters();
	selectedFriendlyCharacter = NULL;
	selectedTargetCharacter = NULL;
	testAnimation = new AnimationObject{ 0, 0, 32, 32, 0, 3, DMG_NO, true, 0.1, 0 };
}

// Game::initCharacters() 
// Initilaisises the two "teams" as arrays of type Character
// Active character list and innactive character lists are swapped at the end of each turn
void Game::initCharacters(){

	Point* blueSpawners = world->getBlueSpawners();
	Point* redSpawners = world->getRedSpawners();
	int x, y;
	
	//initialise active team (blue team) from the list of blue spawners
	x = blueSpawners[0].x;
	y = blueSpawners[0].y;
	activeCharacterList[0] = new Character(x, y, 0, x * tileSize, y * tileSize, 4, FIGHTER, "Blue Fighter");

	x = blueSpawners[1].x;
	y = blueSpawners[1].y;
	activeCharacterList[1] = new Character(x, y, 0, x * tileSize, y * tileSize, 4, FIGHTER, "Blue Fighter");

	x = blueSpawners[2].x;
	y = blueSpawners[2].y;
	activeCharacterList[2] = new Character(x, y, 0, x * tileSize, y * tileSize, 4, FIGHTER, "Blue Fighter");

	x = blueSpawners[3].x;
	y = blueSpawners[3].y;
	activeCharacterList[3] = new Character(x, y, 0, x * tileSize, y * tileSize, 6, SPEARMAN, "Blue Spearman");

	x = blueSpawners[4].x;
	y = blueSpawners[4].y;
	activeCharacterList[4] = new Character(x, y, 0, x * tileSize, y * tileSize, 8, ARCHER, "Blue Archer" );

	
	//now initialise the inactive (red) team from the list of red spawners
	x = redSpawners[0].x;
	y = redSpawners[0].y;
	inactiveCharacterList[0] = new Character(x, y, 0, x * tileSize, y * tileSize, 5, FIGHTER, "Red Fighter");

	x = redSpawners[1].x;
	y = redSpawners[1].y;
	inactiveCharacterList[1] = new Character(x, y, 0, x * tileSize, y * tileSize, 3, WIZARD, "Red Wizard");

	x = redSpawners[2].x;
	y = redSpawners[2].y;
	inactiveCharacterList[2] = new Character(x, y, 0, x * tileSize, y * tileSize, 7, SPEARMAN, "Red Spearman");
	
	x = redSpawners[3].x;
	y = redSpawners[3].y;
	inactiveCharacterList[3] = new Character(x, y, 0, x * tileSize, y * tileSize, 9, ARCHER, "Red Archer");

	x = redSpawners[4].x;
	y = redSpawners[4].y;
	inactiveCharacterList[4] = new Character(x,y, 0, x * tileSize, y * tileSize, 1, KNIGHT, "Red Knight");
}

// Game::switchCharacterLists()
// Simply calls std::swap on the two character lists effectivly switching player control
void Game::switchCharacterLists(){
	std::swap(activeCharacterList, inactiveCharacterList);
}

// Game::endTurn()
// Calls the behaviour for the end of the turn. 
// Resets the characters in both lists and calls switchCharacterLists()
void Game::endTurn(){
	for (int i = 0; i < teamSize; i++){
		activeCharacterList[i]->reset();
		inactiveCharacterList[i]->reset();
	}
	switchCharacterLists();
	selectedFriendlyCharacter = NULL;
	selectedTargetCharacter = NULL;
	world->clearAll();
}

// Game::selectFriendlyCharacter(int mouseX, int mouseY)
// Itterates through the active (friendly to the current player) character lists calling checking their clickedOn function
// returns true if a character was selected false if not.
bool Game::selectFriendlyCharacter(int mouseX, int mouseY){
	for (int i = 0; i < teamSize; i++){
		if (activeCharacterList[i]->clickedOn(mouseX, mouseY, renderer->getRenderOffsetX(), renderer->getRenderOffsetY()) && activeCharacterList[i]->isDead() == false){
			selectedFriendlyCharacter = activeCharacterList[i];
			getRanges(selectedFriendlyCharacter);

			world->selectTile(selectedFriendlyCharacter->getWorldX(), selectedFriendlyCharacter->getWorldY());

			return true;
		}
	}
	return false;
}

// Game::selectTargetCharacter(int mouseX, int mouseY)
// Same as selectFriendlyCharacter just for selecting enemy characters - used for finding the
// target of doCombat()
bool Game::selectTargetCharacter(int mouseX, int mouseY){
	for (int i = 0; i < teamSize; i++){
		if (inactiveCharacterList[i]->clickedOn(mouseX, mouseY, renderer->getRenderOffsetX(), renderer->getRenderOffsetY())){
			selectedTargetCharacter = inactiveCharacterList[i];
			return true;
		}
	}
	return false;
}

// Game::doCombat(Character* friendly, Character* target)
// Takes a friendly character and a target character
// Makes and applies damage caluclations
// Sends the appropriate animation objects to the renderer
// THIS FUNCTION IS DOING TO MUCH - TURN INTO SMALLER FUNCTIONS
// REDUCING THE CHARACTERS POINTS SHOULD BE HANDLED BY THIS FUNCITON - MOVE FUNCTIONALITY FROM CHARACTER CLASS TO GAME
// CHARACTER SHOULD NOT HAVE KNOWLEDGE OF THE GAME RULES - SIMPLY STORE VALUES
void Game::doCombat(Character* friendly, Character* target){
	world->clearAll();

	int dmg = friendly->attack();
	target->doDamage(dmg);

	if (friendly->getProf() == WIZARD){
		renderer->addAnimationObject(
			target->getScreenX() + renderer->getRenderOffsetX() + 24,
		 	target->getScreenY() + renderer->getRenderOffsetY() + 24 - (target->getWorldZ() * 32), 
		 	MAGIC_ATTACK, "");
	}
	else if( friendly->getProf() == FIGHTER || friendly->getProf() == KNIGHT){
		renderer->addAnimationObject(
			target->getScreenX() + renderer->getRenderOffsetX() + 24,
			target->getScreenY() + renderer->getRenderOffsetY() + 24 - (target->getWorldZ() * 32),
			SWORD_ATTACK, "");
	}
	else if(friendly->getProf() == SPEARMAN){
		std::cout << "Spear attack sent\n";
		renderer->addAnimationObject(
			target->getScreenX() + renderer->getRenderOffsetX() + 24,
			target->getScreenY() + renderer->getRenderOffsetY() + 24 - (target->getWorldZ() * 32),
			SPEAR_ATTACK, "");
	}
	else if(friendly->getProf() == ARCHER){
		std::cout << "arrow attack sent\n";
		renderer->addAnimationObject(
			target->getScreenX() + renderer->getRenderOffsetX() + 24,
			target->getScreenY() + renderer->getRenderOffsetY() + 24 - (target->getWorldZ() * 32),
			ARROW_ATTACK, "");
	}
	renderer->addAnimationObject(
		target->getScreenX() + renderer->getRenderOffsetX() + 48,
	 	target->getScreenY() + renderer->getRenderOffsetY() - (target->getWorldZ() * 32),
		DMG_NO, std::to_string(dmg));
}


// Game::getRanges(Character* c)
// This function tells the world object to apply the ranges of the character
// This function is questionable, look into it later
void Game::getRanges(Character* c){

	world->clearAll();
	
	if (c->getMovePoints() > 0){
		world->checkMovementRange(c->getMoveRange(), c->getWorldX(), c->getWorldY());
	}
	if (c->getAttkPoints() > 0){
		world->checkAttackRange(c->getAttkRange(), c->getWorldX(), c->getWorldY());
	}

	world->setOccupiedTiles(activeCharacterList, inactiveCharacterList);
}

// Game::characterInThatPosition(int worldX, int worldY)
// this function takes world coords and returns true or false if a character occupies that space
// questionable
bool Game::characterInThatPosition(int worldX, int worldY){
	for (int i = 0; i < teamSize; i++){
		if (activeCharacterList[i]->getWorldX() == worldX){
			if (activeCharacterList[i]->getWorldY() == worldY){
				return true;
			}
		}
		if (inactiveCharacterList[i]->getWorldX() == worldX){
			if (inactiveCharacterList[i]->getWorldY() == worldY){
				return true;
			}
		}
	}
	return false;
}

void Game::processInputs(InputState inputState){
	switch(currentState){
		case MAINMENU: processInputsMainMenu(inputState); break;
		case GAMEPLAY: processInputsGameplay(inputState); break;
		default: break;
	}
}



//BUG HERE
//if you have a character selected then you click on a tile AND a friendly
// it will selelct the friendly then if you click back on the original character it will instantly move them and display their ranges at
// their old position
void Game::processInputsGameplay(InputState inputState){
	/*
	 if there is no selected character
	 	select a character?
	 if there is a character selected
	 	if the click is on a new char
	 		select that char
	 		exit loop?
	 	if the click is on a moveable tile?
	 		get the path to that tile
	 		assign that path to the character
	 	if the click is on an attackable enemy
	 		do the combat
	*/

	if(inputState.quit) globalRunning = false;

	if(selectedFriendlyCharacter == NULL){
		if(inputState.mouseButtonDown){
			if(selectFriendlyCharacter(inputState.mouseX, inputState.mouseY)){
				//what do we do here?
			}
		}
	}

	if(selectedFriendlyCharacter != NULL){
		if(inputState.mouseButtonDown){
			
			if(selectFriendlyCharacter(inputState.mouseX, inputState.mouseY)){
				//selecting a new friendly character
			}

			else if(selectTargetCharacter(inputState.mouseX, inputState.mouseY)){
				if(selectedFriendlyCharacter->getAttkPoints() > 0){
					if(world->getTileWorldCoords(selectedTargetCharacter->getWorldX(), selectedTargetCharacter->getWorldY()).attackRange){
						// do the combat
						doCombat(selectedFriendlyCharacter, selectedTargetCharacter);
					}
				}
			}
			
			else if(selectedFriendlyCharacter->getMovePoints() > 0){
				mapTile clickedTile = world->getTile(inputState.mouseX, inputState.mouseY, renderer->getRenderOffsetX(), renderer->getRenderOffsetY());
				if(clickedTile.exists && clickedTile.moveRange){
					if(!characterInThatPosition(clickedTile.worldX, clickedTile.worldY)){
						// get the path to that tile and assign it to our character
						selectedFriendlyCharacter->assignPath(
							world->getPath(
								selectedFriendlyCharacter->getWorldX(), selectedFriendlyCharacter->getWorldY(),
								clickedTile.worldX, clickedTile.worldY, selectedFriendlyCharacter->getMoveRange()));
						selectedFriendlyCharacter->setMovePoints(-1);
						world->clearAll();
					}
				}
			}
		}
	}

	if (inputState.space){
		endTurn();
	}

	if(inputState.mouseX < 20){
		if(renderer->getRenderOffsetX() < 1280){
			renderer->addOffsetX(scrollSpeed);
		}
	}
	if(inputState.mouseX > screenW - 20){
		if(renderer->getRenderOffsetX() > 0){
			renderer->addOffsetX(-scrollSpeed);
		}
	}
	if(inputState.mouseY < 20){
		if(renderer->getRenderOffsetY() < 400){
			renderer->addOffsetY(scrollSpeed);
		}
	}
	if(inputState.mouseY > screenH - 20){
		if(renderer->getRenderOffsetY() > -700){
			renderer->addOffsetY(-scrollSpeed);
		}
	}  
}

void Game::processInputsMainMenu(InputState inputState){
	ui->hover(input->getCurrentInputState().mouseX, input->getCurrentInputState().mouseY);

		if (input->getCurrentInputState().mouseButtonDown){
			if (ui->getAction(input->getCurrentInputState().mouseX, input->getCurrentInputState().mouseY) == CHANGESTATE_GAMEPLAY){
				changeState(GAMEPLAY);
			}
			else if (ui->getAction(input->getCurrentInputState().mouseX, input->getCurrentInputState().mouseY) == QUIT){
				globalRunning = false;
			}
		}

}


void Game::update(){
	if(selectedFriendlyCharacter != NULL){
		selectedFriendlyCharacter->animateAlongPath();
	}
}
/*
// Game::update(InputState inputState)
// NEEDS LOOKING AT URGENTLY - DOES TOO MUCH STUFF
// currently this function deals with the majority of the game logic including selection behaviours
// very verbose and untidy - needs looking at
// split differen update behaviours into different functions.
void Game::update(InputState inputState){
	
	if (inputState.quit){
		globalRunning = false;
	}

	if (currentState == GAMEPLAY){
	
		if (selectedFriendlyCharacter == NULL){
			if (inputState.mouseButtonDown){
				if (selectFriendlyCharacter(inputState.mouseX, inputState.mouseY)){
					
					getRanges(selectedFriendlyCharacter);
				}
			}
		}
		else if (selectedFriendlyCharacter != NULL){
			if (inputState.mouseButtonDown){
				mapTile clickedTile = world->getTile(inputState.mouseX, inputState.mouseY, renderer->getRenderOffsetX(), renderer->getRenderOffsetY());
				if (clickedTile.exists){
					if (clickedTile.moveRange){
						if (selectedFriendlyCharacter->getMovePoints() > 0){
							if (!characterInThatPosition(clickedTile.worldX, clickedTile.worldY)){
								printf("move to\n");
								std::vector<Point> path = world->getPath(selectedFriendlyCharacter->getWorldX(), 
																		selectedFriendlyCharacter->getWorldY(), 
																		clickedTile.worldX, 
																		clickedTile.worldY, 
																		selectedFriendlyCharacter->getMoveRange());
								selectedFriendlyCharacter->assignPath(path);
								world->clearAll();
								selectedFriendlyCharacter->setMovePoints(-1);
								for(int a = 0; a < path.size(); a++){
									std::cout << path.at(a).x << ", " << path.at(a).y << std::endl;
								}
								//selectedFriendlyCharacter->moveTo(clickedTile.worldX, clickedTile.worldY, clickedTile.worldZ);
							}
							else printf("that position is blocked\n");
						}
					}
				}
				if (selectTargetCharacter(inputState.mouseX, inputState.mouseY)){
					if (selectedFriendlyCharacter->getAttkPoints() > 0){
						if (selectedTargetCharacter->isDead() != true){
							mapTile enemyPosition = world->getTileWorldCoords(selectedTargetCharacter->getWorldX(), selectedTargetCharacter->getWorldY());
							if (enemyPosition.attackRange == true){
								doCombat(selectedFriendlyCharacter, selectedTargetCharacter);
							}
						}

					}
				}

				if (selectFriendlyCharacter(inputState.mouseX, inputState.mouseY)){
					getRanges(selectedFriendlyCharacter);
				}
			}
		}


		if (inputState.space){
			endTurn();
		}

		if (inputState.up){
			renderer->addOffsetY(scrollSpeed);
		}
		if (inputState.down){
			renderer->addOffsetY(-scrollSpeed);
		}
		if (inputState.left){
			renderer->addOffsetX(scrollSpeed);
		}
		if (inputState.right){
			renderer->addOffsetX(-scrollSpeed);
		}

		if(inputState.mouseX < 20){
			if(renderer->getRenderOffsetX() < 1280){
				renderer->addOffsetX(scrollSpeed);
			}
		}
		if(inputState.mouseX > screenW - 20){
			if(renderer->getRenderOffsetX() > 0){
				renderer->addOffsetX(-scrollSpeed);
			}
		}
		if(inputState.mouseY < 20){
			if(renderer->getRenderOffsetY() < 400){
				renderer->addOffsetY(scrollSpeed);
			}
		}
		if(inputState.mouseY > screenH - 20){
			if(renderer->getRenderOffsetY() > -700){
				renderer->addOffsetY(-scrollSpeed);
			}
		}  

		for (int i = 0; i < teamSize; i++){
			if (!activeCharacterList[i]->isIdle() && activeCharacterList[i]->getMovePoints() < 1 && activeCharacterList[i]->getAttkPoints() < 1 ){
				activeCharacterList[i]->setIdle(true);
			}
			if (!inactiveCharacterList[i]->isIdle() && inactiveCharacterList[i]->getMovePoints() < 1 && activeCharacterList[i]->getAttkPoints() < 1){
				inactiveCharacterList[i]->setIdle(true);
			}
		}
		if(selectedFriendlyCharacter != NULL){
			selectedFriendlyCharacter->animateAlongPath(renderer->getRenderOffsetX(), renderer->getRenderOffsetY());
		}


	}
	else if (currentState == MAINMENU){

		ui->hover(input->getCurrentInputState().mouseX, input->getCurrentInputState().mouseY);

		if (input->getCurrentInputState().mouseButtonDown){
			if (ui->getAction(input->getCurrentInputState().mouseX, input->getCurrentInputState().mouseY) == CHANGESTATE_GAMEPLAY){
				changeState(GAMEPLAY);
			}
			else if (ui->getAction(input->getCurrentInputState().mouseX, input->getCurrentInputState().mouseY) == QUIT){
				globalRunning = false;
			}
		}

	}
	
}
*/
// Game::changeState(GameState newState)
// Deals with changing state and any behaviour specific to a state transition
void Game::changeState(GameState newState){
	if (newState == GAMEPLAY){
		ui->initGameplayElements();
		currentState = GAMEPLAY;
	}
	else if (newState == MAINMENU){
		//do the shit when needed;
	}
}

// Game::gameLoop()
// main game loop
void Game::gameLoop(){


	while (globalRunning){
		
		input->handleEvents();
		processInputs(input->getCurrentInputState());
		update(); 
		//renderMap();
		if (currentState == GAMEPLAY){
			renderer->renderGame(world->map, world->getMapWidth(), world->getMapHeight(), activeCharacterList, inactiveCharacterList, selectedFriendlyCharacter, ui->getElementList(),testAnimation, input->getCurrentInputState().mouseX, input->getCurrentInputState().mouseY);
		}
		else if (currentState == MAINMENU){
			renderer->renderMainMenu(ui->getElementList(), input->getCurrentInputState().mouseX, input->getCurrentInputState().mouseY);
		}
	}
}
