#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "component.hpp"
#include <vector>

class gameObject
{
  int x, y;
public:
  int getX() { return x; }
  int getY() { return y; }
  void setX(int xLoc) { x = xLoc; }
  void setY(int yLoc) { y = yLoc; }

  //The position the game object will be rendered in, lower numbers rendered first (behind)
  int zOrder = 0;
  std::vector<component> components;
  bool hasComponent(int componentID);
};

bool gameObject::hasComponent(int componentID)
{
  for(std::vector<int>::iterator i = components.begin(); i != components.end(); ++i)
  {
    if(componentID == *i)return true;
  }
  return false;
}

#endif
