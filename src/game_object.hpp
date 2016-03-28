#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "component.hpp"
#include <vector>

class gameObject
{
  int x, y;
  //The position the game object will be rendered in, lower numbers rendered first (behind)
  float zOrder = 0;
public:
  gameObject() : x(0), y(0), zOrder(0.0f) {}
  gameObject(int nx, int ny, float nz): x(nx), y(ny), zOrder(nz) {}
  int getX() { return x; }
  int getY() { return y; }
  float getZ() { return zOrder; }
  void setX(int xLoc) { x = xLoc; }
  void setY(int yLoc) { y = yLoc; }
  void setZ(float newZ) { zOrder = newZ; }

  std::vector<component> components;
  bool hasComponent(int componentID);
  component *getComponent(int componentID);
};

bool gameObject::hasComponent(int componentID)
{
  for(std::vector<component>::iterator i = components.begin(); i != components.end(); ++i)
  {
    if(componentID == i->id)return true;
  }
  return false;
}

//Note that pointers can change if the vector is resized etc.
component *gameObject::getComponent(int componentID)
{
  for(std::vector<component>::iterator i = components.begin(); i != components.end(); ++i)
  {
    if(componentID == i->id)return &(*i);
  }
  return 0;
}

#endif
