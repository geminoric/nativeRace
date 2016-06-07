#ifndef SHIP_COMPONENTS_HPP
#define SHIP_COMPONENTS_HPP

#include "component.hpp"

class gameObject;

//Set thrust and rotation angle to move ship
class ship : public component
{
  float thrust;
  bool selected;
  gameObject *selectGlow;
  float selectOffsetX, selectOffsetY;
public:
  //If true, ship automatically moves towards target
  bool autoMove;
  int health;
  //Decel max of 1.0, (1 - decel) * vel
  float speedSq, decel, turnRate, accel, velX, velY, rot, targX, targY;
  gameObject *owner;

  ship(int hp, float acceleration, float deceleration, float maxSpeedSq, float turnSpeed,
    gameObject *parent, float selectOffsetX_ = 0.0f, float selectOffsetY_ = 0.0f);
  ~ship();
  void onUpdate();
  //-1.0 - 1.0
  void setThrust(float thrust_);
  bool isSelected() { return selected; }
  void setSelected(bool sel);

  const char *getCompName() { return "ship"; }
};

#endif
