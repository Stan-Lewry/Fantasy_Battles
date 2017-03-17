#include "Game.h"

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


void Game::initCharacters(){
	activeCharacterList[0] = new Character(0, 6, 0, 0 * tileSize, 6 * tileSize, 0, KNIGHT, "Blue Knight");
	activeCharacterList[1] = new Character(0, 7, 0, 0 * tileSize, 7 * tileSize,  2, WIZARD, "Blue Wizard");
	activeCharacterList[2] = new Character(0, 8, 0, 0 * tileSize, 8 * tileSize, 4, FIGHTER, "Blue Fighter");

	inactiveCharacterList[0] = new Character(14, 6, 0, 14 * tileSize, 6 * tileSize, 1, KNIGHT, "Red Knight");
	inactiveCharacterList[1] = new Character(14, 7, 0, 14 * tileSize, 7 * tileSize, 3, WIZARD, "Red Wizard");
	inactiveCharacterList[2] = new Character(14, 8, 0, 14 * tileSize, 8 * tileSize, 5, FIGHTER, "Red Fighter");
}

void Game::switchCharacterLists(){ //NEEDS FIXING
	//Character* tempCharacterList[teamSize] = activeCharacterList;
	//activeCharacterList = inactiveCharacterList;
	//inactiveCharacterList = tempCharacterList;
	std::swap(activeCharacterList, inactiveCharacterList);
}

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

bool Game::selectFriendlyCharacter(int mouseX, int mouseY){
	for (int i = 0; i < teamSize; i++){
		if (activeCharacterList[i]->clickedOn(mouseX, mouseY, renderer->getRenderOffsetX(), renderer->getRenderOffsetY()) && activeCharacterList[i]->isDead() == false){
			selectedFriendlyCharacter = activeCharacterList[i];
			return true;
		}
	}
	return false;
}

bool Game::selectTargetCharacter(int mouseX, int mouseY){
	for (int i = 0; i < teamSize; i++){
		if (inactiveCharacterList[i]->clickedOn(mouseX, mouseY, renderer->getRenderOffsetX(), renderer->getRenderOffsetY())){
			selectedTargetCharacter = inactiveCharacterList[i];
			printf("YOU CLICKED ON AN EMEMY\n");
			return true;
		}
	}
	return false;
}

void Game::doCombat(Character* friendly, Character* target){
	world->clearAll();

	int dmg = friendly->attack();
	target->doDamage(dmg);

	

	if (friendly->getProfession() == "wizard"){
		renderer->addAnimationObject(target->getScreenX() + renderer->getRenderOffsetX() + 48,
		 target->getScreenY() + renderer->getRenderOffsetY() + 48 - (target->getWorldZ() * 16) , MAGIC_ATTACK, "");
	}
	else{
		renderer->addAnimationObject(target->getScreenX() + renderer->getRenderOffsetX() + 48,
		 target->getScreenY() + renderer->getRenderOffsetY() + 48 - (target->getWorldZ() * 16) , SWORD_ATTACK, "");
	}
	renderer->addAnimationObject(target->getScreenX() + renderer->getRenderOffsetX() + 48,
	 				target->getScreenY() + renderer->getRenderOffsetY() - (target->getWorldZ() * 16),
					 DMG_NO, std::to_string(dmg));
	

	//friendly->setAttkPoints(-1);
}

void Game::getRanges(Character* c){

	world->clearAll(); //??????
	
	if (c->getMovePoints() > 0){
		world->checkMovementRange(c->getMoveRange(), c->getWorldX(), c->getWorldY());
	}
	if (c->getAttkPoints() > 0){
		world->checkAttackRange(c->getAttkRange(), c->getWorldX(), c->getWorldY());
	}
}

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

		/*
		
		if no character selected
			get click
			if click is on a friendly character
				that character is currently selected
				if attack & move points > 0
					display there ranges

		if a character is selected
			get click
			if that click is on a tile thats in move range
				if move points > 0
					if that tile is not the another characrers x/y
						move character to that tile
			if that click is on an enemy character
				if attack points > 0
					if that char is in attack range
						attack that character --- perhaps show a menu?
			if that click is on a friendly character
				select that character
		*/


		if (inputState.space){
			//endTurn();
			//testAnimation->dead = false;
			//testAnimation->animationFrame = 0;	
			//testAnimation->screenX = 300;
			//testAnimation->screenY = 300;
			//renderer->addAnimationObject(380, 400, SWORD_ATTACK, "");
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


void Game::changeState(GameState newState){
	if (newState == GAMEPLAY){
		ui->initGameplayElements();
		currentState = GAMEPLAY;
	}
	else if (newState == MAINMENU){
		//do the shit when needed;
	}
}


void Game::gameLoop(){
	//world->loadMap("Levels/test.level");


	//ui->initGameplayElements();



	while (globalRunning){
		/*
		oldTime = currentTime;
		currentTime = SDL_GetTicks();
		ftime = (currentTime - oldTime) / 1000.0f;
	
		frames++;
		timer += ftime;
		if (timer > 1){
			std::cout << frames << std::endl;
			frames = 0;
			timer = 0;
		}
		*/
		
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
