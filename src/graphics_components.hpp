#ifndef GRAPHICS_COMPONENTS_HPP
#define GRAPHICS_COMPONENTS_HPP

#include "component.hpp"

class render : public component
{
public:
  render();
  void onUpdate() {}
  void onStart() {}
  //Used to increment the id only when a new component type is created
  static bool idAlreadyCreated;
};

#endif
