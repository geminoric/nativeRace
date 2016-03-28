# Macros ========================================

CC=g++-4.9
CFLAGS=-c -g -Wall -ansi -pedantic -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter -std=gnu++11

ERASE=rm
OBJECTS=$(DIR)main.o $(DIR)graphics_core.o $(DIR)graphics_components.o $(DIR)component.o $(DIR)game.o
DIR=gnu/
EXE=nativeRace

# Targets ========================================

$(EXE) : $(OBJECTS)
	$(CC) -o $(EXE) $(OBJECTS) -lsfml-graphics -lsfml-window -lsfml-system

$(DIR)main.o : main.cpp
	$(CC) -o $(DIR)main.o $(CFLAGS) main.cpp
  
$(DIR)graphics_core.o : graphics_core.cpp graphics_core.hpp
	$(CC) -o $(DIR)graphics_core.o $(CFLAGS) graphics_core.cpp

$(DIR)graphics_components.o : graphics_components.cpp graphics_components.hpp
	$(CC) -o $(DIR)graphics_components.o $(CFLAGS) graphics_components.cpp

$(DIR)component.o : component.cpp component.hpp
	$(CC) -o $(DIR)component.o $(CFLAGS) component.cpp

$(DIR)game.o : game.cpp game.hpp
	$(CC) -o $(DIR)game.o $(CFLAGS) game.cpp

clean :
	$(ERASE) $(EXE) $(OBJECTS)