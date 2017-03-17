#include "Input.h"

Input::Input(){
	inputState = { false, 0, 0, false, false, false, false, false, false};
}

void Input::handleEvents(){
	inputState = { false, 0, 0, false, false, false, false, false, false };

	SDL_GetMouseState(&inputState.mouseX, &inputState.mouseY);

	while (SDL_PollEvent(&evnt)){
		switch (evnt.type){
		case SDL_QUIT:
			inputState.quit = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputState.mouseButtonDown = true;
			//SDL_GetMouseState(&inputState.mouseX, &inputState.mouseY);
			break;
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym){
			case SDLK_SPACE:
				inputState.space = true;
				break;
			case SDLK_UP:
				inputState.up = true;
				break;
			case SDLK_DOWN:
				inputState.down = true;
				break;
			case SDLK_LEFT:
				inputState.left = true;
				break;
			case SDLK_RIGHT:
				inputState.right = true;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}

InputState Input::getCurrentInputState(){
	return inputState;
}