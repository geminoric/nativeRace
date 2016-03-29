#ifndef GRAPHICS_COMPONENTS_HPP
#define GRAPHICS_COMPONENTS_HPP

#include "component.hpp"
#include "graphics_core.hpp"

namespace sf
{
  class Texture;
}

class render : public component
{
public:
  render();
  render(sf::Texture *texture);
  render(const char *textName);
  ~render();
  void setTexture(sf::Texture *texture) { comptexture = texture; }
  void onUpdate() {}
  void onStart() {}
  const char *getCompName() { return "render"; }
  //Used to increment the id only when a new component type is created
  static bool idAlreadyCreated;
  sf::Texture *comptexture;
};

#endif
