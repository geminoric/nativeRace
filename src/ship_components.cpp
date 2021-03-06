#include <iostream>
#include <cmath>
#include <vector>
#include "ship_components.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "graphics_components.hpp"
#include "collision.hpp"
#include "particles.hpp"

#define ANGLE_EPSILON 0.2
#define TARGET_EPSILON_SQ 40000
#define DISTANCE_DECREASE_THRUST_SQ 90000
#define DISTANCE_BUMP_STOP 160000
#define BUMP_FRAME_TO_STOP 120

namespace statusValues
{
  extern int mouseX;
  extern int mouseY;
  extern float timeMultiplier;
}

namespace gameControl
{
  extern std::vector<gameObject *> gameObjects;
}

ship::ship(int hp, float acceleration, float deceleration, float maxSpeedSq, 
  float turnSpeed, gameObject *parent, float selectOffsetX_, float selectOffsetY_, int faction) : rot(0.0f), thrust(0.0f),
  health(hp), accel(acceleration), turnRate(turnSpeed), speedSq(maxSpeedSq), velX(0.0f), velY(0.0f), owner(parent),
  decel(deceleration), selectOffsetX(selectOffsetX_), selectOffsetY(selectOffsetY_), autoMove(false), consecBump(0),
  factionID(faction), updateTargCount(0), enemyTarget(0), shootCooldown(0)
{
  render *pRender = parent->getComponent<render>("render");
  //Create collision circle
  owner->addComponent(new circleCollision(pRender->textXSize / 2, owner, 100, 1));
  circleCollision *pCol = owner->getComponent<circleCollision>("circleCollision");
  pCol->addCollisionID(1);
  pCol->bumpable = true;

  //Create animation
  pThrust = gameControl::createObject(owner->x, owner->y, 29.8f);
  pThrust->addComponent(new shipThrust(pRender->textYSize, owner, pThrust, "Thrust_08"));

  //Create selection glow
  selectGlow = gameControl::createObject(0, 0, 29.7f);
  selectGlow->addComponent(new render("selectGlow", 0, 256, 0, 256, pRender->textXSize, pRender->textYSize, 255, 255, 255, 0));
  targX = owner->x;
  targY = owner->y;
}

ship::~ship()
{
  //Check if other ships are targeting this, if so delete the target
  for(std::vector<gameObject *>::iterator i = gameControl::gameObjects.begin();i != gameControl::gameObjects.end();++i)
  {
    if(*i == owner)continue;

    ship *pShip = (*i)->getComponent<ship>("ship");
    if(pShip && pShip->enemyTarget == owner)
      pShip->enemyTarget = 0;
  }
  gameControl::deleteObject(selectGlow);
  gameControl::deleteObject(pThrust);
  pThrust->getComponent<shipThrust>("shipThrust")->parent = 0;
}

void ship::checkUpdateEnemyTarget()
{
  if(updateTargCount <= 0)
  {
    enemyTarget = 0;
    for(std::vector<gameObject *>::iterator i = gameControl::gameObjects.begin();i != gameControl::gameObjects.end();++i)
    {
      ship *pShip = (*i)->getComponent<ship>("ship");
      if(!pShip || pShip->factionID == factionID)continue;

      float distSq = ((*i)->x - owner->x) * ((*i)->x - owner->x) + ((*i)->y - owner->y) * ((*i)->y - owner->y);

      if(distSq < 10000000)
      {
        enemyTarget = *i;
        break;
      }
    }

    updateTargCount = 60;
  }
  --updateTargCount;
}

void ship::checkShoot()
{
  if(shootCooldown <= 0 && enemyTarget)
  {
    gameObject *pbullet = gameControl::createObject(owner->x, owner->y, owner->zOrder - 0.001f);
    pbullet->addComponent(new render("Fire_02", 0, 248, 0, 248, 128, 128));
    pbullet->addComponent(new bullet(enemyTarget->x - owner->x, enemyTarget->y - owner->y, pbullet, factionID));
    pbullet->addComponent(new circleCollision(64, pbullet));
    pbullet->getComponent<circleCollision>("circleCollision")->addCollisionID(1);
    shootCooldown = rand() % 100 + 200;
  }
  --shootCooldown;
  if(shootCooldown < 0)shootCooldown = 0;
}

void ship::checkDie()
{
  if(health <= 0)
  {
    //Create particles
    for(int i = 0;i < 25;++i)
    {
      gameObject *part = gameControl::createObject(owner->x, owner->y, owner->zOrder + 0.001f);
      part->addComponent(new particle(part, rand() % 30 - 15, rand() % 30 - 15, 0.999, 2, "Fire_02", 248, 248, 124));
    }

    gameControl::deleteObject(owner);
  }
}

void ship::onUpdate()
{
  checkUpdateEnemyTarget();
  checkShoot();

  if(autoMove)
  {
    //Decelerate
    velX *= (1.0f - decel);
    velY *= (1.0f - decel);

    float angleToTarget = atan2(targY - owner->y, targX - owner->x);
    if(rot > 3.1415f)rot = -2.0f * 3.14159265f + rot;
    if(rot < -3.1415f)rot = 2.0f * 3.14159265f + rot;

    float distSq = (targX - owner->x) * (targX - owner->x) + (targY - owner->y) * (targY - owner->y);

    if(consecBump > BUMP_FRAME_TO_STOP && distSq < DISTANCE_BUMP_STOP)
    {
      thrust = 0.0f;
      autoMove = false;
    }
    else if(distSq > TARGET_EPSILON_SQ)
    {
      float angleDif = angleToTarget - rot;

      //Don't turn if close enough angle
      if(angleDif > ANGLE_EPSILON || angleDif < -ANGLE_EPSILON)
      {
        if(angleDif > 3.141592f)angleDif = -2.0f * 3.141592f + angleDif;
        else if(angleDif < -3.141592f)angleDif = 2.0f * 3.141592f + angleDif;

        if(angleDif > 0)rot += turnRate;
        else
          rot -= turnRate;
      }

      if(distSq < DISTANCE_DECREASE_THRUST_SQ)thrust = 0.2f;
      else
        thrust = 1.0f;
    }
    else
    {
      if(angleToTarget - rot > ANGLE_EPSILON)autoMove = false;
      thrust = 0.0f;
    }
  }

  //Update movement
  velX += cos(rot) * accel * thrust;
  velY += sin(rot) * accel * thrust;

  //Cap speed
  float speedFactor = (velX * velX + velY * velY) / speedSq;
  if(speedFactor > 1.2f)
  {
    velX /= 1.0f + speedFactor * 0.2f;
    velY /= 1.0f + speedFactor * 0.2f;
  }
  else if(speedFactor > 1.0f)
  {
    velX /= speedFactor;
    velY /= speedFactor;
  }

  //Decelerate quicker if no thrust
  if(!thrust)
  {
    velX *= (1.0f - decel);
    velY *= (1.0f - decel);
  }

  //Move based on velocity
  owner->x += velX;
  owner->y += velY;

  //Update selection glow position
  selectGlow->x = owner->x + selectOffsetX * cos(rot + 3.141592f);
  selectGlow->y = owner->y + selectOffsetY * sin(rot + 3.141592f);

  //Update graphical stuff
  owner->getComponent<render>("render")->rot = rot + 3.141592f / 2.0f;

  checkDie();
}

void ship::setThrust(float thrust_)
{
  thrust = thrust_;
  if(thrust > 1.0f)thrust = 1.0f;
  if(thrust < -1.0f)thrust = -1.0f;
}

void ship::setSelected(bool sel)
{
  selected = sel;
  if(sel)
  {
    selectGlow->getComponent<render>("render")->alpha = 255;
  }
  else
  {
    selectGlow->getComponent<render>("render")->alpha = 0;
  }
}


shipThrust::shipThrust(int thrustOffset, gameObject *parent_, gameObject *owner_, const char *textName) : parent(parent_), owner(owner_),
  parentXOffset(owner_->x - parent_->x), parentYOffset(owner_->y - parent_->y), parentThrustOffset(thrustOffset)
{
  owner->addComponent(new render(textName, 0, 512, 0, 512, 128, 128));
  pRender = owner->getComponent<render>("render");
}

void shipThrust::onUpdate()
{
  if(!parent)
  {
    pRender->alpha = 0;
    return;
  }
  ship *pShip = parent->getComponent<ship>("ship");
  float rot = pShip->rot;
  //Move to ship
  owner->x = parent->x + (parentXOffset - pShip->selectOffsetX + parentThrustOffset) * cos(rot + 3.141592f);
  owner->y = parent->y + (parentYOffset - pShip->selectOffsetY + parentThrustOffset) * sin(rot + 3.141592f);

  pRender->rot = rot + 3.141592f / 2.0f;

  if(!pShip->getThrust())
  {
    pRender->alpha = 0;
  }
  else
  {
    render *parRender = parent->getComponent<render>("render");
    //Create particles
    gameObject *part = gameControl::createObject(parent->x + parRender->textXSize / 2.0f + rand() % 30 - 15, 
      parent->y + parRender->textYSize / 2.0f + (sin(rot + 3.141592f) * parentYOffset) + rand() % 30 - 15, 29.6f);
    part->addComponent(new particle(part, -pShip->velX, -pShip->velY, 0.9f, 5, "Thrust_Particle", 43, 42, 42));
    pRender->alpha = 255;
  }
}

void bullet::onUpdate()
{
  ++distTravelled;
  float mag = sqrt(velX * velX + velY * velY);
  owner->x += velX / mag * 50;
  owner->y += velY / mag * 50;
  if(distTravelled > 3000)gameControl::deleteObject(owner);
}
