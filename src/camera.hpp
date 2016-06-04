#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "component.hpp"

class camera : public component
{
public:
  bool isActive;
  float camX;
  float camY;
  float camSpeed;

  camera(float x, float y, bool active);
  void updateCameraPos();

  void onUpdate();

  //Used to increment the id only when a new component type is created
  static bool idAlreadyCreated;
  const char *getCompName() { return "camera"; }
};

#endif
