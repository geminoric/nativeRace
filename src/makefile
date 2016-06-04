# Macros ========================================

CC=g++-4.9
CFLAGS=-c -g -O2 -Wall -ansi -pedantic -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter -std=gnu++11 -Wno-reorder

ERASE=rm
OBJECTS=$(DIR)main.o $(DIR)graphics_core.o $(DIR)graphics_components.o $(DIR)component.o $(DIR)game.o $(DIR)game_object.o $(DIR)background.o $(DIR)camera.o $(DIR)eventHandler.o
DIR=gnu/
EXE=nativeRace

# Targets ========================================

$(EXE) : $(OBJECTS)
	$(CC) -o $(EXE) $(OBJECTS) -lsfml-graphics -lsfml-window -lsfml-system

$(DIR)main.o : main.cpp graphics_core.hpp game.hpp game_object.hpp background.hpp camera.hpp eventHandler.hpp
	$(CC) -o $(DIR)main.o $(CFLAGS) main.cpp
  
$(DIR)graphics_core.o : graphics_core.cpp graphics_core.hpp graphics_components.hpp game.hpp game_object.hpp
	$(CC) -o $(DIR)graphics_core.o $(CFLAGS) graphics_core.cpp

$(DIR)graphics_components.o : graphics_components.cpp graphics_components.hpp
	$(CC) -o $(DIR)graphics_components.o $(CFLAGS) graphics_components.cpp

$(DIR)component.o : component.cpp component.hpp
	$(CC) -o $(DIR)component.o $(CFLAGS) component.cpp

$(DIR)game.o : game.cpp game.hpp game_object.hpp
	$(CC) -o $(DIR)game.o $(CFLAGS) game.cpp

$(DIR)game_object.o : game_object.cpp game_object.hpp
	$(CC) -o $(DIR)game_object.o $(CFLAGS) game_object.cpp

$(DIR)background.o : background.cpp background.hpp
	$(CC) -o $(DIR)background.o $(CFLAGS) background.cpp

$(DIR)camera.o : camera.cpp camera.hpp
	$(CC) -o $(DIR)camera.o $(CFLAGS) camera.cpp

$(DIR)eventHandler.o : eventHandler.cpp eventHandler.hpp
	$(CC) -o $(DIR)eventHandler.o $(CFLAGS) eventHandler.cpp


clean :
	$(ERASE) $(EXE) $(OBJECTS)