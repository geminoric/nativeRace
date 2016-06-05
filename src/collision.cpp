#include <iostream>
#include "collision.hpp"

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
