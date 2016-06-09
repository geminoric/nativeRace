#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <vector>
#include "game_object.hpp"
#include "component.hpp"

//---------------------------
//COLLISION ID MASTER LIST
//1 - Ships
//
//
//---------------------------


class circleCollision : public component
{
  gameObject *owner;

  void checkBumping();
  void checkBulletCol();
  float isCollidingDistRatio(circleCollision *pOther);
public:
  //If this is true object will bounce off other bumpable objects
  bool bumpable;
  int radius;
  //100 mass is somewhat small, 100,000 is pretty big
  int mass;
  //-1 collisionID for no collisionID collision
  int collisionID;
  std::vector<int> collisionIDList;

  circleCollision(int radius_, gameObject *owner_, int mass_ = 100, int colID = -1);
  bool isColliding(gameObject *other);
  bool isColliding(circleCollision *other);
  void checkPlanetCapture();
  void addCollisionID(int id) { collisionIDList.push_back(id); }

  //Checks for bumping into things
  void onUpdate();

  const char *getCompName() { return "circleCollision"; }
};

#endif
