#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "component.hpp"
#include <vector>
#include <string.h>
#include <memory>

class gameObject
{
  int x, y;
  //The position the game object will be rendered in, lower numbers rendered first (behind)
  float zOrder = 0;
public:
  gameObject() : x(0), y(0), zOrder(0.0f) {}
  gameObject(int nx, int ny, float nz): x(nx), y(ny), zOrder(nz) {}
  int getX() const { return x; }
  int getY() const { return y; }
  float getZ() const { return zOrder; }
  void setX(int xLoc) { x = xLoc; }
  void setY(int yLoc) { y = yLoc; }
  void setZ(float newZ) { zOrder = newZ; }

  std::vector<std::shared_ptr<component>> components;
  bool hasComponent(int componentID);
  //Return 0 if component not found
  template <typename T> T *getComponent(int componentID);
  template <typename T> T *getComponent(const char *componentName);
};

//Note that pointers can change if the vector is resized etc.
template <typename T> T *gameObject::getComponent(int componentID)
{
  for(std::vector<std::shared_ptr<component>>::iterator i = components.begin(); i != components.end(); ++i)
  {
    if(componentID == (*i)->id)return dynamic_cast<T *>(&(**i));
  }
  return 0;
}

template <typename T> T *gameObject::getComponent(const char *componentName)
{
  for(std::vector<std::shared_ptr<component>>::iterator i = components.begin(); i != components.end(); ++i)
  {
    if(!strcmp(componentName, (*i)->getCompName()))return dynamic_cast<T *>(&(**i));
  }
  return 0;
}

#endif
