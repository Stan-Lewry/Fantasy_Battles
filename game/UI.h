// UI Class
// This class deals with initliasing UI objects for each game state
// Also handles inputs and animaitons on UI elements
// Details on implementation can be found in UI.cpp

#ifndef UI_HEADER
#define UI_HEADER

#include "SDLUtils.h"

class UI{
public:
	UI();
	void initGameplayElements();

	void initMainMenuElements();

	void initStageSelectionElements();

	void initPauseElements();

	UIElement* getElementList();
	UIElement* getPauseElementsList();

	UIAction getAction(int mouseX, int mouseY);

	UIAction getPauseAction(int mouseX, int mouseY);

	void hover(int mouseX, int mouseY);

	void pauseHover(int mouseX, int mouseY);
private:
	UIElement elements[7];
	UIElement pauseElements[3];
};

#endif