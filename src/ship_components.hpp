#ifndef SHIP_COMPONENTS_HPP
#define SHIP_COMPONENTS_HPP

#include "component.hpp"

class gameObject;
class render;

//Set thrust and rotation angle to move ship
class ship : public component
{
  void checkUpdateEnemyTarget();
  void checkShoot();
  void checkDie();

  float thrust;
  bool selected;
  gameObject *selectGlow, *pThrust;
  int updateTargCount, shootCooldown;
public:
  gameObject *enemyTarget;
  int factionID;
  //Consecutive frames this ship has been bumping
  int consecBump;
  //If true, ship automatically moves towards target
  bool autoMove;
  int health;
  //Decel max of 1.0, (1 - decel) * vel
  float speedSq, decel, turnRate, accel, velX, velY, rot, targX, targY, selectOffsetX, selectOffsetY;
  gameObject *owner;

  ship(int hp, float acceleration, float deceleration, float maxSpeedSq, float turnSpeed,
    gameObject *parent, float selectOffsetX_ = 0.0f, float selectOffsetY_ = 0.0f, int faction = 0);
  ~ship();
  void onUpdate();
  //-1.0 - 1.0
  void setThrust(float thrust_);
  float getThrust() { return thrust; }
  bool isSelected() { return selected; }
  void setSelected(bool sel);

  const char *getCompName() { return "ship"; }
};

//Thrust animation + effects
class shipThrust : public component
{
public:
  gameObject *parent;
  gameObject *owner;
  render *pRender;
  int parentXOffset;
  int parentYOffset;
  int parentThrustOffset;

  shipThrust(int thrustOffset, gameObject *parent_, gameObject *owner_, const char *textName);
  void onUpdate();

  const char *getCompName() { return "shipThrust"; }
};

class bullet : public component
{
public:
  float velX, velY;
  gameObject *owner;
  int facOwner, distTravelled;

  bullet(float vX, float vY, gameObject *parent, int facID) : velX(vX), velY(vY), owner(parent), facOwner(facID), distTravelled(0) {}

  void onUpdate();

  const char *getCompName() { return "bullet"; }
};

#endif
