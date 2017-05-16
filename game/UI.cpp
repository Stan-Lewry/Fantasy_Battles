#include "UI.h"

// UI::UI()
// constructor - apparently this does nothing
UI::UI(){
	//initElements();
}


// void UI::initGameplayElements()
// This functions initialises all the ui elements required for ui during gameplay
void UI::initGameplayElements(){
	
	UIElement mainWindow = { 0, 0, 300, 720, 0, 0, A };
	elements[0] = mainWindow;

	UIElement profIcon = { 50, 220, 32, 32, 524,0, false, NO_ACTION };
	elements[1] = profIcon;

	UIElement HPIcon = { 50, 284, 32, 32, 364, 0, false, NO_ACTION };
	elements[2] = HPIcon;

	UIElement attackIcon = { 50, 348, 32, 32, 396, 0, false, NO_ACTION };
	elements[3] = attackIcon;

	UIElement defenseIcon = { 50, 412, 32, 32, 428, 0, false, NO_ACTION };
	elements[4] = defenseIcon;

	UIElement mpIcon = { 50, 476, 32, 32, 460, 0, false, NO_ACTION };
	elements[5] = mpIcon;

	UIElement apIcon = { 50, 540, 32, 32, 492, 0, false, NO_ACTION };
	elements[6] = apIcon;
	
	
}

// void UI::initMainMenuElements(){
// initlialises the ui elements for the main menu
void UI::initMainMenuElements(){

	for(int i = 0; i < 6; i++){
		elements[i] = {0,0,0,0,0,0,false, NO_ACTION};
	}

	UIElement play = { screenW - 364, 500, 300, 64, 0, 0, false, CHANGESTATE_STAGESELECT };
	elements[0] = play;

	//UIElement editor = { screenW - 364, 328, 300, 64, 0, 64, false, NO_ACTION };
	//elements[1] = editor;

	//UIElement options = { screenW - 364, 465, 300, 64, 0, 128, false, NO_ACTION };
	//elements[2] = options;

	UIElement exit = { screenW - 364, 584, 300, 64, 0, 192, false, QUIT };
	elements[3] = exit;
}

void UI::initStageSelectionElements(){

	//screenX, screenY, width, height, sourceX, sourceY, hover, function

	std::cout << "initStageSelectionElements" << std::endl;

	UIElement castle = {700, 216, 240, 80, 0, 0, false, LOADLEVEL_CASTLE};
	
	UIElement planes = {270, 450, 240, 80, 0, 80, false, LOADLEVEL_PLANES};

	UIElement arena = {760, 446, 240, 80, 0, 160, false, LOADLEVEL_ARENA};

	UIElement bridge = {370, 174, 240, 80, 0, 240, false, LOADLEVEL_BRIDGE};

	elements[0] = castle;
	elements[1] = planes;
	elements[2] = arena;
	elements[3] = bridge;
}

void UI::initPauseElements(){
	UIElement resume = {440, 300, 400, 80, 0, 0, false, PAUSE_RESUME};
	UIElement mainMenu = {440, 400, 400, 80, 0, 80, false, PAUSE_MAINMENU};
	UIElement quitGame = {440, 500, 400, 80, 0, 160, false, PAUSE_QUITGAME};

	pauseElements[0] = resume;
	pauseElements[1] = mainMenu;
	pauseElements[2] = quitGame;
}

// UIElement* UI::getElementList()
// Returns the vector of UI elements as it currently is
UIElement* UI::getElementList(){
	return elements;
}

UIElement* UI::getPauseElementsList(){
	return pauseElements;
}
// UIAction UI::getAction(int mouseX, int mouseY)
// checks if the mouse coords are within a ui elements and returns the assiciated UIAction
UIAction UI::getAction(int mouseX, int mouseY){
	for (int i = 0; i < 6; i++){
		if (mouseX >= elements[i].screenX && mouseX < elements[i].screenX + elements[i].width){
			if (mouseY >= elements[i].screenY && mouseY < elements[i].screenY + elements[i].height){
				return elements[i].action;
			}
		}
	}
	return NO_ACTION;
}

UIAction UI::getPauseAction(int mouseX, int mouseY){
	for (int i = 0; i < 2; i++){
		if (mouseX >= pauseElements[i].screenX && mouseX < pauseElements[i].screenX + pauseElements[i].width){
			if (mouseY >= pauseElements[i].screenY && mouseY < pauseElements[i].screenY + pauseElements[i].height){
				return pauseElements[i].action;
			}
		}
	}
	return NO_ACTION;
}
// void UI::hover(int mouseX, int mouseY)
// toggles a ui elements hover bool if the mouse coords are within a button 
void UI::hover(int mouseX, int mouseY){
	for (int i = 0; i <6; i++){
		if (mouseX >= elements[i].screenX && mouseX < elements[i].screenX + elements[i].width){
			if (mouseY >= elements[i].screenY && mouseY < elements[i].screenY + elements[i].height){
				for (int x = 0; x < 4; x++){
					elements[x].hover = false;
				}
				elements[i].hover = true;
			}
		}
	}
	
}

void UI::pauseHover(int mouseX, int mouseY){
	for (int i = 0; i <3; i++){
		if (mouseX >= pauseElements[i].screenX && mouseX < pauseElements[i].screenX + pauseElements[i].width){
			if (mouseY >= pauseElements[i].screenY && mouseY < pauseElements[i].screenY + pauseElements[i].height){
				for (int x = 0; x < 4; x++){
					pauseElements[x].hover = false;
				}
				pauseElements[i].hover = true;
			}
		}
	}	
}