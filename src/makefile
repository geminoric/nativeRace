# Macros ========================================

CC=g++-4.9
CFLAGS=-c -g -O2 -Wall -ansi -pedantic -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter -std=gnu++11 -Wno-reorder

ERASE=rm
OBJECTS=$(DIR)main.o $(DIR)graphics_core.o $(DIR)graphics_components.o $(DIR)component.o $(DIR)game.o $(DIR)game_object.o $(DIR)background.o $(DIR)camera.o $(DIR)eventHandler.o $(DIR)structure_components.o $(DIR)collision.o $(DIR)ui.o $(DIR)empire_components.o $(DIR)initLogic.o $(DIR)sector.o $(DIR)ship_components.o $(DIR)selection.o
DIR=gnu/
EXE=nativeRace

# Targets ========================================

$(EXE) : $(OBJECTS)
	$(CC) -o $(EXE) $(OBJECTS) -lsfml-graphics -lsfml-window -lsfml-system

$(DIR)main.o : main.cpp graphics_core.hpp game.hpp game_object.hpp background.hpp camera.hpp eventHandler.hpp ui.hpp initLogic.hpp sector.hpp
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

$(DIR)background.o : background.cpp background.hpp game_object.hpp game.hpp graphics_components.hpp
	$(CC) -o $(DIR)background.o $(CFLAGS) background.cpp

$(DIR)camera.o : camera.cpp camera.hpp
	$(CC) -o $(DIR)camera.o $(CFLAGS) camera.cpp

$(DIR)eventHandler.o : eventHandler.cpp eventHandler.hpp
	$(CC) -o $(DIR)eventHandler.o $(CFLAGS) eventHandler.cpp

$(DIR)structure_components.o : structure_components.cpp structure_components.hpp
	$(CC) -o $(DIR)structure_components.o $(CFLAGS) structure_components.cpp

$(DIR)collision.o : collision.cpp collision.hpp
	$(CC) -o $(DIR)collision.o $(CFLAGS) collision.cpp

$(DIR)ui.o : ui.cpp ui.hpp empire_components.hpp game_object.hpp game.hpp graphics_components.hpp
	$(CC) -o $(DIR)ui.o $(CFLAGS) ui.cpp

$(DIR)empire_components.o : empire_components.cpp empire_components.hpp
	$(CC) -o $(DIR)empire_components.o $(CFLAGS) empire_components.cpp

$(DIR)initLogic.o : initLogic.cpp initLogic.hpp ship_components.hpp
	$(CC) -o $(DIR)initLogic.o $(CFLAGS) initLogic.cpp

$(DIR)sector.o : sector.cpp sector.hpp
	$(CC) -o $(DIR)sector.o $(CFLAGS) sector.cpp

$(DIR)ship_components.o : ship_components.cpp ship_components.hpp
	$(CC) -o $(DIR)ship_components.o $(CFLAGS) ship_components.cpp

$(DIR)selection.o : selection.cpp selection.hpp
	$(CC) -o $(DIR)selection.o $(CFLAGS) selection.cpp

clean :
	$(ERASE) $(EXE) $(OBJECTS)