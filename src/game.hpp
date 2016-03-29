#ifndef GAME_HPP
#define GAME_HPP

#include <vector>

class gameObject;

namespace gameControl
{
  extern std::vector<gameObject *> gameObjects;
  //Store functions to be called at start and end of level
  extern void(* levelStart)();
  extern void(* levelEnd)();

  //Functions for controlling the current level
  void loadLevel(void(* startLevel)(), void(* destroyLevel)());
  void switchLevel(void(* newStartLevel)(), void(* newDestroyLevel)());
  void reloadLevel();
  void unloadLevel();
  //Call to unload the level with a custom function
  void unloadLevel(void(* destroyLevel)());
  //Returns the created object
  gameObject * createObject(int x = 0, int y = 0, float z = 0.0f);
  void deleteAllObjects();
}

#endif
