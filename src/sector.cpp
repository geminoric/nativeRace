#include <iostream>
#include "sector.hpp"
#include "game_object.hpp"
#include "game.hpp"

sector::~sector()
{
  //Delete all objects in the sector
  for(std::vector<gameObject *>::iterator i = objectsInSector.begin();i != objectsInSector.end();++i)
  {
    gameControl::deleteObject((*i));
  }
}

void sector::addToSector(gameObject *obj)
{
  objectsInSector.push_back(obj);
}
