#include <vector>
#include <memory>
#include <iostream>
#include "game.hpp"
#include "game_object.hpp"
#include "graphics_components.hpp"
#include "graphics_core.hpp"

namespace
{
  std::vector<gameObject *> objectQueue;
  std::vector<gameObject *> deleteQueue;
}

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
  gameObject *createObject(int x, int y, float z)
  {
    gameObject *newobj = new gameObject(x, y, z);
    objectQueue.push_back(newobj);
    return newobj;
  }

  //Adds the current game object queue to list and then clears the queue
  void addQueueToList()
  {
    for(std::vector<gameObject *>::iterator j = objectQueue.begin();j != objectQueue.end();++j)
    {
      //Need to insert the first object manually
      if(gameObjects.empty())
      {
        gameObjects.push_back(*j);
        continue;
      }
      //Add the object to object list in correct z order
      for(std::vector<gameObject *>::iterator i = gameObjects.begin(); i != gameObjects.end(); ++i)
      {
        //Find place to insert
        if((*j)->zOrder < (*i)->getZ())
        {
          gameObjects.insert(i, *j);
          break;
        }
        else if (i + 1 == gameObjects.end())
        {
          //Add to end if it's leftover
          gameObjects.push_back(*j);
          break;
        }
      }
    }
    objectQueue.clear();
  }

  void deleteAllObjects()
  {
    while(!gameObjects.empty())
    {
      delete gameObjects.back();
      gameObjects.pop_back();
    }
  }

  void deleteObject(gameObject *obj)
  {
    deleteQueue.push_back(obj);
  }

  //Delete all objects in the delete queue and then clear the queue
  void deleteObjectQueue()
  {
    if(deleteQueue.empty())return;

    for(std::vector<gameObject *>::iterator i = gameObjects.begin(); i != gameObjects.end(); ++i)
    {
      for(std::vector<gameObject *>::iterator j = deleteQueue.begin();j != deleteQueue.end();++j)
      {
        if(*i == *j)
        {
          delete *i;
          *i = 0;
          i = gameObjects.erase(i) - 1;
          break;
        }
      }
    }

    deleteQueue.clear();
  }

  void runObjectUpdate()
  {
    addQueueToList();
    deleteObjectQueue();

    for(std::vector<gameObject *>::iterator i = gameObjects.begin(); i != gameObjects.end(); ++i)
    {
      for(std::vector<std::shared_ptr<component>>::iterator j = (*i)->components.begin(); j != (*i)->components.end(); ++j)
      {
        (*j)->onUpdate();
      }
    }
  }
}
