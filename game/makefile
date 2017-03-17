OBJS = Character.cpp Character.h Game.cpp Game.h Input.cpp Input.h Main.cpp Renderer.cpp Renderer.h SDLUtils.cpp SDLUtils.h UI.cpp UI.h World.cpp World.h

CC = g++

COMPILER_FLAGS = -w -std=c++11

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

OBJ_NAME = fantasy_battles

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

