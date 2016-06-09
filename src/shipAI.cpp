#include <iostream>
#include <vector>
#include "shipAI.hpp"
#include "ship_components.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "structure_components.hpp"

namespace gameControl
{
  extern std::vector<gameObject *> gameObjects;
}

void shipAI::onUpdate()
{
  //Update target to closest player planet every 100 frames
  if(curUpdate >= maxUpdate)
  {
    float minDistSq = 9999999999.0f;
    float minTarX = 0.0f, minTarY = 0.0f;
    for(std::vector<gameObject *>::iterator i = gameControl::gameObjects.begin(); i != gameControl::gameObjects.end();++i)
    {
      planet *pPlanet = (*i)->getComponent<planet>("planet");
      //Player planet
      if(pPlanet && pPlanet->ownerID == 0)
      {
        float dist = ((*i)->x - owner->x) * ((*i)->x - owner->x) + ((*i)->y - owner->y) * ((*i)->y - owner->y);
        if(dist < minDistSq)
        {
          minDistSq = dist;
          minTarX = (*i)->x + pPlanet->planetSize / 2.0f;
          minTarY = (*i)->y + pPlanet->planetSize / 2.0f;
        }
      }
    }

    if(minDistSq < 9999999999 && minDistSq > 1000.0f)
    {
      pShip->targX = minTarX;
      pShip->targY = minTarY;
      pShip->autoMove = true;
    }

    curUpdate = 0;
  }
  ++curUpdate;
}


