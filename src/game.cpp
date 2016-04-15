#include <vector>
#include <memory>
#include "game.hpp"
#include "game_object.hpp"

namespace gameControl
{
  std::vector<gameObject *> gameObjects;
  //Store functions to be called at start and end of level
  void(* levelStart)() = 0;
  void(* levelEnd)() = 0;

  //Functions for controlling the current level
  void loadLevel(void(* startLevel)(), void(* destroyLevel)())
  {
    levelStart = startLevel;
    levelEnd = destroyLevel;
    startLevel();
  }

  void switchLevel(void(* newStartLevel)(), void(* newDestroyLevel)())
  {
    levelEnd();
    levelStart = newStartLevel;
    levelEnd = newDestroyLevel;
    newStartLevel();
  }

  void reloadLevel()
  {
    levelEnd();
    levelStart();
  }

  void unloadLevel()
  {
    levelEnd();
    levelStart = 0;
    levelEnd = 0;
  }

  //Call to unload the level with a custom function
  void unloadLevel(void(* destroyLevel)())
  {
    destroyLevel();
    levelStart = 0;
    levelEnd = 0;
  }

  //Returns the created object
  gameObject * createObject(int x, int y, float z)
  {
    gameObject *newobj = new gameObject(x, y, z);
    //Add the object to object list in correct z order
    for(std::vector<gameObject *>::iterator i = gameObjects.begin(); i != gameObjects.end(); ++i)
    {
      //Find place to insert
      if(z < (*i)->getZ())
      {
        gameObjects.insert(i, newobj);
        return newobj;
      }
    }
    //Add to end if it's leftover
    gameObjects.push_back(newobj);
    return newobj;
  }

  void deleteAllObjects()
  {
    while(!gameObjects.empty())
    {
      delete gameObjects.back();
      gameObjects.pop_back();
    }
  }

  void runObjectUpdate()
  {
    for(std::vector<gameObject *>::iterator i = gameObjects.begin(); i != gameObjects.end(); ++i)
    {
      for(std::vector<std::shared_ptr<component>>::iterator j = (*i)->components.begin(); j != (*i)->components.end(); ++j)
      {
        (*j)->onUpdate();
      }
    }
  }
}
