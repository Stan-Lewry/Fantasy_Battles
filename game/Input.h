// Input Class
// This class abstracts the details of input handling from the Game class, collecting inputs in an InputState struct
// which is processed by the game on each frame.
// This abstraction allows an AI to pass inputs to the Game without changing anything in the Game class.
// Details on implementation can be found in Input.cpp

#ifndef INPUT_HEADER
#define INPUT_HEADER

#include "SDLUtils.h"

struct InputState{
	bool mouseButtonDown;
	int mouseX;
	int mouseY;
	bool quit;
	bool space;

	bool up;
	bool down;
	bool left;
	bool right;
};


class Input{
public:
	Input();
	void handleEvents();
	InputState getCurrentInputState();

private:
	SDL_Event evnt;
	InputState inputState;

};

#endif