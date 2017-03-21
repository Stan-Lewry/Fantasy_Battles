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
	world->loadMap("Levels/Mountain.level");
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
	activeCharacterList[0] = new Character(0, 6, 0, 0 * tileSize, 6 * tileSize, 4, FIGHTER, "Blue Fighter");
	activeCharacterList[1] = new Character(0, 7, 0, 0 * tileSize, 7 * tileSize,  4, FIGHTER, "Blue Fighter");
	activeCharacterList[2] = new Character(0, 8, 0, 0 * tileSize, 8 * tileSize, 4, FIGHTER, "Blue Fighter");

	inactiveCharacterList[0] = new Character(14, 6, 0, 14 * tileSize, 6 * tileSize, 3, WIZARD, "Red Wizard");
	inactiveCharacterList[1] = new Character(14, 7, 0, 14 * tileSize, 7 * tileSize, 3, WIZARD, "Red Wizard");
	inactiveCharacterList[2] = new Character(14, 8, 0, 14 * tileSize, 8 * tileSize, 3, WIZARD, "Red Wizard");
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

	if (friendly->getProfession() == "wizard"){
		renderer->addAnimationObject(
			target->getScreenX() + renderer->getRenderOffsetX() + 48,
		 	target->getScreenY() + renderer->getRenderOffsetY() + 48 - (target->getWorldZ() * 16), 
		 	MAGIC_ATTACK, "");
	}
	else{
		renderer->addAnimationObject(
			target->getScreenX() + renderer->getRenderOffsetX() + 48,
			target->getScreenY() + renderer->getRenderOffsetY() + 48 - (target->getWorldZ() * 16),
			SWORD_ATTACK, "");
	}
	renderer->addAnimationObject(
		target->getScreenX() + renderer->getRenderOffsetX() + 48,
	 	target->getScreenY() + renderer->getRenderOffsetY() - (target->getWorldZ() * 16),
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

// Game::update(InputState inputState)
// NEEDS LOOKING AT URGENTLY - DOES TOO MUCH STUFF
// currently this function deals with the majority of the game logic including selection behaviours
// very verbose and untidy - needs looking at
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
								for(int a = 0; a < path.size(); a++){
									std::cout << path.at(a).x << ", " << path.at(a).y << std::endl;
								}
								selectedFriendlyCharacter->moveTo(clickedTile.worldX, clickedTile.worldY, clickedTile.worldZ);
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
		update(input->getCurrentInputState()); 
		//renderMap();
		if (currentState == GAMEPLAY){
			renderer->renderGame(world->map, activeCharacterList, inactiveCharacterList, selectedFriendlyCharacter, ui->getElementList(),testAnimation, input->getCurrentInputState().mouseX, input->getCurrentInputState().mouseY);
		}
		else if (currentState == MAINMENU){
			renderer->renderMainMenu(ui->getElementList(), input->getCurrentInputState().mouseX, input->getCurrentInputState().mouseY);
		}
	}
}
