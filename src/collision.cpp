#include <iostream>
#include "collision.hpp"
#include "game.hpp"
#include "ship_components.hpp"
#include "structure_components.hpp"

#define BUMP_CONSTANT 0.001

namespace gameControl
{
  extern std::vector<gameObject *> gameObjects;
}

namespace playerData
{
  extern resources *playerResources;
}

circleCollision::circleCollision(int radius_, gameObject *owner_, int mass_, int colID) : radius(radius_), owner(owner_), collisionID(colID), mass(mass_), bumpable(false)
{
  if(!mass)
  {
    mass = 1;
    std::cout << "[WARNING] Mass passed as 0, setting to 1\n";
  }
}

bool circleCollision::isColliding(gameObject *other)
{
  circleCollision *pOther = other->getComponent<circleCollision>("circleCollision");
  if((owner->x - other->x) * (owner->x - other->x) + (owner->y - other->y) * (owner->y - other->y) 
    < (radius + pOther->radius) * (radius + pOther->radius))return true;
  return false;
}

bool circleCollision::isColliding(circleCollision *pOther)
{
  if((owner->x - pOther->owner->x) * (owner->x - pOther->owner->x) + (owner->y - pOther->owner->y) * (owner->y - pOther->owner->y) 
    < (radius + pOther->radius) * (radius + pOther->radius))return true;
  return false;
}

//Returns the ratio of sqrt(sum_radius) / sqrt(dist)
//Returns 0 if no collision
float circleCollision::isCollidingDistRatio(circleCollision *pOther)
{
  float distSq = (owner->x - pOther->owner->x) * (owner->x - pOther->owner->x) + (owner->y - pOther->owner->y) * (owner->y - pOther->owner->y);
  float sumRadSq = (radius + pOther->radius) * (radius + pOther->radius);
  if(distSq < sumRadSq && sumRadSq)
  {
    return sumRadSq / distSq;
  }
  return 0.0f;
}

void circleCollision::checkBumping()
{
  if(!bumpable || collisionID == -1 || collisionIDList.empty())return;

  bool bumpedThisFrame = false;

  //Check for bumping with each object
  for(std::vector<gameObject *>::iterator i = gameControl::gameObjects.begin();i != gameControl::gameObjects.end();++i)
  {
    if((*i) == owner)continue;

    circleCollision *pOther = (*i)->getComponent<circleCollision>("circleCollision");
    if(pOther)
    {
      for(std::vector<int>::iterator j = collisionIDList.begin();j != collisionIDList.end();++j)
      {
        float colRatio = isCollidingDistRatio(pOther);
        if(pOther->collisionID == *j && colRatio
)        {
          //Collision detected
          ship *pShip = owner->getComponent<ship>("ship");
          if(pShip)
          {
            if(!bumpedThisFrame)
            {
              pShip->consecBump++;
              bumpedThisFrame = true;
            }

            float deltaX = (owner->x - (*i)->x) * BUMP_CONSTANT;
            float deltaY = (owner->y - (*i)->y) * BUMP_CONSTANT;

            float massScale = pOther->mass / mass;
            pShip->velX += deltaX * massScale * colRatio * colRatio;
            pShip->velY += deltaY * massScale * colRatio * colRatio;
          }
          else
            std::cout << "[WARNING] Bump detected with local object not ship\n";
        }
      }
    }
  }
  //Reset counter
  if(!bumpedThisFrame)
  {
    owner->getComponent<ship>("ship")->consecBump = 0;
  }
}

void circleCollision::checkPlanetCapture()
{
  ship *pShip = owner->getComponent<ship>("ship");
  if(pShip)
  {
    for(std::vector<gameObject *>::iterator i = gameControl::gameObjects.begin();i != gameControl::gameObjects.end();++i)
    {
      if((*i) == owner)continue;
      planet *pPlanet = (*i)->getComponent<planet>("planet");
      if(!pPlanet)continue;

      if(isColliding(*i))
      {
        if(pShip->factionID != 0)pPlanet->deCapture(pShip->factionID);
        else
          //Temp, replace so it works with not the player lul
          pPlanet->capture(pShip->factionID, playerData::playerResources);      }
    }
  }
}

void circleCollision::checkBulletCol()
{
  bullet *pBullet = owner->getComponent<bullet>("bullet");
  if(!pBullet)return;
  for(std::vector<gameObject *>::iterator i = gameControl::gameObjects.begin();i != gameControl::gameObjects.end();++i)
  {
    if((*i) == owner)continue;
    ship *pShip = (*i)->getComponent<ship>("ship");
    if(!pShip)continue;

    if(pBullet->facOwner != pShip->factionID && isColliding(*i))
    {
      pShip->health -= 5;
      gameControl::deleteObject(owner);
      return;
    }
  }
}

void circleCollision::onUpdate()
{
  checkBulletCol();
  checkPlanetCapture();
  checkBumping();
}
