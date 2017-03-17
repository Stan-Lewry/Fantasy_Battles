#ifndef UI_HEADER
#define UI_HEADER

#include "SDLUtils.h"

class UI{
public:
	UI();
	void initGameplayElements();

	void initMainMenuElements();

	UIElement* getElementList();

	UIAction getAction(int mouseX, int mouseY);

	void hover(int mouseX, int mouseY);

private:
	UIElement elements[7];
};

#endif