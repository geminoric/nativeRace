#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "component.hpp"
#include <vector>
#include <string.h>
#include <memory>

class gameObject
{
public:
  float x, y;
  //The position the game object will be rendered in, lower numbers rendered first (behind)
  float zOrder = 0;
  gameObject() : x(0.0f), y(0.0f), zOrder(0.0f) {}
  gameObject(int nx, int ny, float nz): x(nx), y(ny), zOrder(nz) {}
  float getX() const { return x; }
  float getY() const { return y; }
  float getZ() const { return zOrder; }
  void setX(int xLoc) { x = xLoc; }
  void setY(int yLoc) { y = yLoc; }
  void setZ(float newZ) { zOrder = newZ; }

  std::vector<std::shared_ptr<component>> components;
  void addComponent(component *newComponent);
  bool hasComponent(int componentID);
  //Return 0 if component not found
  template <typename T> T *getComponent(const char *componentName);
};

template <typename T> T *gameObject::getComponent(const char *componentName)
{
  for(std::vector<std::shared_ptr<component>>::iterator i = components.begin(); i != components.end(); ++i)
  {
    if(!strcmp(componentName, (*i)->getCompName()))return dynamic_cast<T *>(&(**i));
  }
  return 0;
}

#endif
