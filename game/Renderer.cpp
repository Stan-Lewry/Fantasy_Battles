#include "Renderer.h"

Renderer::Renderer(SDL_Renderer* _rend){
	rend = _rend;
	initTextures();
	initFonts();
}

void Renderer::renderGame(mapTile map[mapH][mapW], Character* renderableCharacters1[teamSize], Character* renderableCharacters2[teamSize], Character* currentCharacter, UIElement renderableUIElements[1], AnimationObject* animationObject, int mouseX, int mouseY){
	
	oldTime = currentTime;
	currentTime = SDL_GetTicks();
	ftime = (currentTime - oldTime) / 1000.0f;

	frames++;
	timer += ftime;

	
	SDL_RenderClear(rend);
	renderMapBackground();
	renderWorld(map, currentCharacter, renderableCharacters1, renderableCharacters2);
	//renderCharacters(renderableCharacters1, map);
	//renderCharacters(renderableCharacters2, map);
	renderStatusBars(renderableCharacters1);
	renderStatusBars(renderableCharacters2);
	renderUI(renderableUIElements, currentCharacter);
	renderCursor(mouseX, mouseY);
	renderAnimationObjects(animationObject);
	//renderText("CUM", font, 600, 600, 255, 0, 0);

	if (timer > 1){

		//std::stringstream sStream;
		//stream << frames;
		//printf("frame > 1\n");
		//std::string fString = std::to_string(frames);
		//const char* fChar = fString.c_str();
		//printf(fChar);
		//renderText("CUM", font, 600, 600, 255, 0, 0);
		printf("%iFPS\n", frames);
		frames = 0;
		timer = 0;
	}

	SDL_RenderPresent(rend);
}

void Renderer::renderWorld(mapTile map[mapH][mapW], Character* currentCharacter, Character* charList1[teamSize], Character* charList2[teamSize]){
	SDL_Rect sRect = { 0, 0, worldSpriteSize, worldSpriteSize };
	SDL_Rect dRect = { 0, 0, 256, 256 };
	
	for (int i = 0; i < mapH; i++){
		for (int j = 0; j < mapW; j++){
			
			int height = map[i][j].worldZ * 16;
			
			dRect.x = map[i][j].screenX + renderOffsetX - 64;
			dRect.y = map[i][j].screenY + renderOffsetY - 64;
			dRect.y -= height;
			sRect.x = map[i][j].typeX * 2;
			sRect.y = map[i][j].typeY * 2;
			SDL_RenderCopy(rend, worldSpriteSheet, &sRect, &dRect); 

			if (currentCharacter != NULL){
				if (currentCharacter->getAttkRange() > currentCharacter->getMoveRange()){
					if (map[i][j].moveRange){

						sRect.y = 1 * worldSpriteSize;
						sRect.x = 9 * worldSpriteSize;
						SDL_RenderCopy(rend, worldSpriteSheet, &sRect, &dRect);
					}
					else if (map[i][j].attackRange){
	
						sRect.y = 1 * worldSpriteSize;
						sRect.x = 8 * worldSpriteSize;
						SDL_RenderCopy(rend, worldSpriteSheet, &sRect, &dRect);
					}
				}
				else if (currentCharacter->getAttkRange() < currentCharacter->getMoveRange()){
					if (map[i][j].moveRange){
						sRect.y = 1 * worldSpriteSize;
						sRect.x = 9 * worldSpriteSize;
						SDL_RenderCopy(rend, worldSpriteSheet, &sRect, &dRect);
					}
					if (map[i][j].attackRange){
						sRect.y = 1 * worldSpriteSize;
						sRect.x = 8 * worldSpriteSize;
						SDL_RenderCopy(rend, worldSpriteSheet, &sRect, &dRect);
					}
				}
			}


			if (map[i][j].selected){
				sRect.y = 1 * worldSpriteSize;
				sRect.x = 7 * worldSpriteSize;
				SDL_RenderCopy(rend, worldSpriteSheet, &sRect, &dRect);
			}

			for (int k = 0; k < teamSize; k++){
				if (charList1[k]->getWorldX() == j && charList1[k]->getWorldY() == i){
					SDL_Rect sRect = { charList1[k]->getAnimationFrame() * spriteSize, charList1[k]->getSpriteID() * spriteSize, spriteSize, spriteSize };
					SDL_Rect dRect = { charList1[k]->getScreenX() + renderOffsetX, charList1[k]->getScreenY() - height + renderOffsetY, tileSize, tileSize };
					SDL_RenderCopy(rend, characterSpriteSheet, &sRect, &dRect);

					//	SUBTRACT HEIGHT VALUE FROM CHARACTER Y 
				}
				if (charList2[k]->getWorldX() == j && charList2[k]->getWorldY() == i){
					SDL_Rect sRect = { charList2[k]->getAnimationFrame() * spriteSize, charList2[k]->getSpriteID() * spriteSize, spriteSize, spriteSize };
					SDL_Rect dRect = { charList2[k]->getScreenX() + renderOffsetX, charList2[k]->getScreenY() - height + renderOffsetY, tileSize, tileSize };
					SDL_RenderCopy(rend, characterSpriteSheet, &sRect, &dRect);

					// SAME AS ABOVE
				}
			}

			/*
			for (int k = 0; k < teamSize; k++){
				//SDL_Rect sRect = { charList1[i]->getAnimationFrame() * spriteSize, charList1[i]->getSpriteID() * spriteSize, spriteSize, spriteSize };
				SDL_Rect dRect = { charList1[k]->getScreenX() + renderOffsetX, charList1[k]->getScreenY() - height + renderOffsetY, tileSize, tileSize };
				//SDL_RenderCopy(rend, characterSpriteSheet, &sRect, &dRect);

				SDL_Rect sRect = { 364, 32, 27, 4 };
				dRect.x = dRect.x + (tileSize / 2) - 14;
				dRect.y += 38;
				dRect.w = (charList1[k]->getCurrentHP() * 27) / charList1[k]->getMaxHP();
				dRect.h = 4;

				SDL_RenderCopy(rend, uiSpriteSheet, &sRect, &dRect);

				dRect = { charList2[k]->getScreenX() + renderOffsetX, charList2[k]->getScreenY() - height + renderOffsetY, tileSize, tileSize };
				//SDL_RenderCopy(rend, characterSpriteSheet, &sRect, &dRect);

				sRect = { 364, 32, 27, 4 };
				dRect.x = dRect.x + (tileSize / 2) - 14;
				dRect.y += 38;
				dRect.w = (charList2[i]->getCurrentHP() * 27) / charList2[i]->getMaxHP();
				dRect.h = 4;

				SDL_RenderCopy(rend, uiSpriteSheet, &sRect, &dRect);


			}
			*/

		}
	}
}

void Renderer::renderStatusBars(Character* renderableCharacters[teamSize]){
	//std::cout << "render characters called" << std::endl;
	for (int i = 0; i < teamSize; i++){
		SDL_Rect sRect = { renderableCharacters[i]->getAnimationFrame() * spriteSize, renderableCharacters[i]->getSpriteID() * spriteSize, spriteSize, spriteSize };
		SDL_Rect dRect = { renderableCharacters[i]->getScreenX() + renderOffsetX, renderableCharacters[i]->getScreenY() + renderOffsetY - (renderableCharacters[i]->getWorldZ() * 16), tileSize, tileSize };
		//SDL_RenderCopy(rend, characterSpriteSheet, &sRect, &dRect);

		sRect = { 364, 32, 27, 4 };
		dRect.x = dRect.x + (tileSize / 2) - 14;
		dRect.y += 76;
		dRect.w = (renderableCharacters[i]->getCurrentHP() * 27) / renderableCharacters[i]->getMaxHP();
		dRect.h = 4;

		SDL_RenderCopy(rend, uiSpriteSheet, &sRect, &dRect);


	}
}

void Renderer::renderUI(UIElement renderableUIElements[1], Character* currentCharacter){

	//SDL_Rect fpsCounterDest = { 0, 0, 0, 0 };
	//char* text = "TEST TEXT";
	//TTF_SizeText(font, text, &fpsCounterDest.w, &fpsCounterDest.h);
	//renderText
	/*
	SDL_Rect sRect;
	SDL_Rect dRect;
	for (int i = 0; i < 7; i++){
		sRect = { renderableUIElements[i].sourceX, renderableUIElements[i].sourceY, renderableUIElements[i].width, renderableUIElements[i].height };
		dRect = { renderableUIElements[i].screenX, renderableUIElements[i].screenY, renderableUIElements[i].width, renderableUIElements[i].height };
		SDL_RenderCopy(rend, uiSpriteSheet, &sRect, &dRect);
	}
	*/
	
	int uiX = 10;
	int uiY = screenH - 225;	

	SDL_Rect sRect = {0 ,0, 300, 150};
	SDL_Rect dRect = {uiX, uiY, 450, 225};

	SDL_RenderCopy(rend, uiSpriteSheet, &sRect, &dRect);

	if(currentCharacter != NULL){
		sRect = { 2* spriteSize, currentCharacter->getSpriteID() * spriteSize, spriteSize, spriteSize};
		dRect = {uiX - 10, uiY - 20, 225, 225};
		SDL_RenderCopy(rend, characterSpriteSheet, &sRect, &dRect);

		sRect = { 0, 150, 140, 11};
		dRect = {uiX + 190, uiY + 100, 210, 17};
		SDL_RenderCopy(rend, uiSpriteSheet, &sRect, &dRect);
	}
	/*
	if (currentCharacter != NULL){
		sRect = { 2 * spriteSize, currentCharacter->getSpriteID() * spriteSize, spriteSize, spriteSize };
		dRect = { 75, 70, 150, 150 };
		SDL_RenderCopy(rend, characterSpriteSheet, &sRect, &dRect);

		renderText(currentCharacter->getProfession(), font, 86, 220 - 8, 0, 0, 0);

		renderText(std::to_string(currentCharacter->getCurrentHP()).append("/").append(std::to_string(currentCharacter->getMaxHP())).c_str(), font, 86, 284 - 8, 0, 0, 0);

		renderText(std::to_string(currentCharacter->getAttk()).c_str(), font, 86, 348 - 8, 0, 0, 0);

		renderText(std::to_string(currentCharacter->getArmour()).c_str(), font, 86, 412 - 8, 0, 0, 0);

		renderText(std::to_string(currentCharacter->getMovePoints()).c_str(), font, 86, 476 - 8, 0, 0, 0);

		renderText(std::to_string(currentCharacter->getAttkPoints()).c_str(), font, 86, 540 - 8, 0, 0, 0);
	}
	*/

}


void Renderer::renderMapBackground(){
	SDL_Rect dRect = { 0, 0, screenW, screenH };
	SDL_SetRenderDrawColor(rend, 102, 204, 255, 1);
	SDL_RenderFillRect(rend, &dRect);
}


void Renderer::renderCursor(int mouseX, int mouseY){
	SDL_Rect sRect = { 300, 0, 64, 64 };
	SDL_Rect dRect = { mouseX, mouseY, 32, 32 };
	SDL_RenderCopy(rend, uiSpriteSheet, &sRect, &dRect);

}

void Renderer::renderText(const char* text, TTF_Font* font, int x, int y, int r, int g, int b){
	SDL_Rect dest = { x, y, 0, 0 };
	TTF_SizeText(font, text, &dest.w, &dest.h);	
	SDL_Color col = { r, g, b };
	SDL_Surface* tempSurf = TTF_RenderText_Solid(font, text, col);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, tempSurf);
	SDL_FreeSurface(tempSurf);
	SDL_RenderCopy(rend, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
}


void Renderer::renderAnimationObjects(AnimationObject* object){

	

	if (animationStack.size() > 0){
		for (int i = 0; i < animationStack.size(); i++){

			if (animationStack.at(i).animType == DMG_NO){
				
				animationStack.at(i).frameTimer += ftime;



				renderText(animationStack.at(i).text.c_str(), dmgFont, animationStack.at(i).screenX, animationStack.at(i).screenY, 255, 0, 0);

				if (animationStack.at(i).frameTimer > animationStack.at(i).frameSpeed){
					animationStack.at(i).screenY -= 1;
					animationStack.at(i).animationFrame += 1;
					animationStack.at(i).frameTimer = 0;
					
				}
				if (animationStack.at(i).animationFrame > animationStack.at(i).endFrame){
					animationStack.erase(animationStack.begin() + i);
				}
				


			}
			else if (animationStack.at(i).animType == SWORD_ATTACK){
		
				animationStack.at(i).frameTimer += ftime;

				SDL_Rect sRect = { animationStack.at(i).animationFrame * animationStack.at(i).width, 0, animationStack.at(i).width, animationStack.at(i).height };
				SDL_Rect dRect = { animationStack.at(i).screenX, animationStack.at(i).screenY, animationStack.at(i).width, animationStack.at(i).height };
				SDL_RenderCopy(rend, swordAnimationSheet, &sRect, &dRect);

				if (animationStack.at(i).frameTimer > animationStack.at(i).frameSpeed){
					animationStack.at(i).animationFrame += 1;
					animationStack.at(i).frameTimer = 0;
				}

				if (animationStack.at(i).animationFrame > animationStack.at(i).endFrame){
			
					animationStack.erase(animationStack.begin() + i);
				}
			}
			else if (animationStack.at(i).animType == MAGIC_ATTACK){
			
				animationStack.at(i).frameTimer += ftime;

				SDL_Rect sRect = { animationStack.at(i).animationFrame * animationStack.at(i).width, 0, animationStack.at(i).width, animationStack.at(i).height };
				SDL_Rect dRect = { animationStack.at(i).screenX, animationStack.at(i).screenY, animationStack.at(i).width, animationStack.at(i).height };
				SDL_RenderCopy(rend, fireAnimationSheet, &sRect, &dRect);

				if (animationStack.at(i).frameTimer > animationStack.at(i).frameSpeed){
					animationStack.at(i).animationFrame += 1;
					animationStack.at(i).frameTimer = 0;
				}

				if (animationStack.at(i).animationFrame > animationStack.at(i).endFrame){
		
					animationStack.erase(animationStack.begin() + i);
				}
			}
		}
	}
}

void Renderer::renderMainMenu(UIElement renderableUIElements[4], int mouseX, int mouseY){
	
	SDL_RenderClear(rend);

	SDL_Rect bgSource = { 0, 0, screenW, screenH };
	SDL_Rect bgDest = { 0, 0, screenW, screenH };
	SDL_RenderCopy(rend, titleScreen, &bgSource, &bgDest);

	SDL_Rect sRect, dRect;
	for (int i = 0; i < 4; i++){
		if (renderableUIElements[i].hover){
			sRect = { renderableUIElements[i].sourceX + 300, renderableUIElements[i].sourceY, renderableUIElements[i].width, renderableUIElements[i].height };
		}
		else{
			sRect = { renderableUIElements[i].sourceX, renderableUIElements[i].sourceY, renderableUIElements[i].width, renderableUIElements[i].height };
		}
		dRect = { renderableUIElements[i].screenX, renderableUIElements[i].screenY, renderableUIElements[i].width, renderableUIElements[i].height };
		SDL_RenderCopy(rend, mainMenuButtons, &sRect, &dRect);
	}
	
	renderCursor(mouseX, mouseY);

	SDL_RenderPresent(rend);
}


SDL_Texture* Renderer::loadPNG(char path[]){
	printf("loading: %s\n", path);
	SDL_Surface* tempSurface = IMG_Load(path);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, tempSurface);
	SDL_FreeSurface(tempSurface);
	printf("Done!\n");
	return texture;
}



void Renderer::initTextures(){
	worldSpriteSheet = loadPNG("Assets/iso_tiles_large.png");
	characterSpriteSheet = loadPNG("Assets/characters_large.png");
	uiSpriteSheet = loadPNG("Assets/ui.png");
	titleScreen = loadPNG("Assets/title_screen.png");
	mainMenuButtons = loadPNG("Assets/main_menu_buttons.png");
	swordAnimationSheet = loadPNG("Assets/sword_anim.png");
	fireAnimationSheet = loadPNG("Assets/fire_anim.png");
}


void Renderer::initFonts(){
	font = TTF_OpenFont("Fonts/font.ttf", 30);
	dmgFont = TTF_OpenFont("Fonts/font.ttf", 18);
}

int Renderer::getRenderOffsetX(){
	return renderOffsetX;
}

int Renderer::getRenderOffsetY(){
	return renderOffsetY;
}

void Renderer::addOffsetX(int a){
	renderOffsetX += a;
}

void Renderer::addOffsetY(int a){
	renderOffsetY += a;
}

int Renderer::getMapSpaceOffset(){
	return mapSpaceOffset;
}


void Renderer::addAnimationObject(int screenX, int screenY, AnimationType animType, std::string text){

	if (animType == DMG_NO){
		AnimationObject newAnimObj = { screenX, screenY, 32, 32, 0, 20, animType,false,  0.05, 0, text };
		animationStack.push_back(newAnimObj);

	}
	else if (animType == SWORD_ATTACK){
		AnimationObject newAnimObj = { screenX, screenY, 32, 32, 0, 8, animType, false, 0.05, 0, text };
		animationStack.push_back(newAnimObj);
	}

	else if (animType == MAGIC_ATTACK){
		AnimationObject newAnimObj = { screenX, screenY, 32, 32, 0, 8, animType, false, 0.05, 0, text };
		animationStack.push_back(newAnimObj);
	}

	
}
