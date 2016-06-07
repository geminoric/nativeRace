#include <iostream>
#include <cmath>
#include "ship_components.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "graphics_components.hpp"

#define ANGLE_EPSILON 0.2
#define TARGET_EPSILON_SQ 25000
#define DISTANCE_DECREASE_THRUST_SQ 5500

namespace statusValues
{
  extern int mouseX;
  extern int mouseY;
}

ship::ship(int hp, float acceleration, float deceleration, float maxSpeedSq, 
  float turnSpeed, gameObject *parent, float selectOffsetX_, float selectOffsetY_) : rot(0.0f), thrust(0.0f),
  health(hp), accel(acceleration), turnRate(turnSpeed), speedSq(maxSpeedSq), velX(0.0f), velY(0.0f), owner(parent),
  decel(deceleration), selectOffsetX(selectOffsetX_), selectOffsetY(selectOffsetY_), autoMove(true)
{
  //Create selection glow
  selectGlow = gameControl::createObject(0, 0, 9.9f);
  render *pRender = parent->getComponent<render>("render");
  selectGlow->addComponent(new render("selectGlow", 0, 256, 0, 256, pRender->sizeX, pRender->sizeY, 255, 255, 255, 0));
  targX = owner->x;
  targY = owner->y;
}

ship::~ship()
{
  gameControl::deleteObject(selectGlow);
}

void ship::onUpdate()
{
  if(autoMove)
  {
    float angleToTarget = atan2(targY - owner->y, targX - owner->x);
    if(rot > 3.1415f)rot = -3.1415f + rot;
    if(rot < -3.1415f)rot = 3.1415f - rot;

    float distSq = (targX - owner->x) * (targX - owner->x) + (targY - owner->y) * (targY - owner->y);
    if(distSq > TARGET_EPSILON_SQ)
    {
      float angleDif = angleToTarget - rot;

      //Don't turn if close enough angle
      if(angleDif > ANGLE_EPSILON || angleDif < -ANGLE_EPSILON)
      {
        if(angleDif > 3.1415f)angleDif = -3.1415f + angleDif;
        else if(angleDif < -3.1415f)angleDif = 3.1415f - angleDif;

        if(angleDif > 0)rot += turnRate;
        else
          rot -= turnRate;
      }

      if(distSq < DISTANCE_DECREASE_THRUST_SQ)thrust = 0.4f;
      else
        thrust = 1.0f;
    }
    else
      thrust = 0.0f;
  }

  //Update movement
  velX += cos(rot) * accel * thrust;
  velY += sin(rot) * accel * thrust;

  //Cap speed
  float speedFactor = (velX * velX + velY * velY) / speedSq;
  if(speedFactor > 1)
  {
    velX /= speedFactor;
    velY /= speedFactor;
  }

  //Decelerate if no thrust
  if(!thrust)
  {
    velX *= (1.0f - decel);
    velY *= (1.0f - decel);
  }

  //Move based on velocity
  owner->x += velX;
  owner->y += velY;

  //Update selection glow position
  selectGlow->x = owner->x + selectOffsetX;
  selectGlow->y = owner->y + selectOffsetY;

  //Update graphical stuff
  owner->getComponent<render>("render")->rot = rot + 3.141592f / 2.0f;

  //Check if the ship dies
  if(health < 0)gameControl::deleteObject(owner);
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
