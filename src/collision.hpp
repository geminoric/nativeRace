#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "game_object.hpp"
#include "component.hpp"

class circleCollision : public component
{
  gameObject *owner;
public:
  int radius;
  circleCollision(int radius_, gameObject *owner_) : radius(radius_), owner(owner_) {}
  bool isColliding(gameObject *other);
  bool isColliding(circleCollision *other);

  const char *getCompName() { return "circleCollision"; }
};

#endif
