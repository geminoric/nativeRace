#ifndef GRAPHICS_COMPONENTS_HPP
#define GRAPHICS_COMPONENTS_HPP

#include <vector>
#include "component.hpp"
#include "graphics_core.hpp"

namespace sf
{
  class Texture;
}

class gameObject;

class render : public component
{
public:
  float scaleX;
  float scaleY;
  float sizeX;
  float sizeY;
  //Used to increment the id only when a new component type is created
  static bool idAlreadyCreated;

  render(float sizX, float sizY, float scalX = 1.0f, float scalY = 1.0f);
  render(sf::Texture *texture, float sizX, float sizY, float scaleX = 1.0f, float scaleY = 1.0f);
  render(const char *textName, float sizX, float sizY, float scaleX = 1.0f, float scaleY = 1.0f);
  ~render();
  void setTexture(sf::Texture *texture) { comptexture = texture; }
  void onUpdate() {}
  void onStart() {}
  const char *getCompName() { return "render"; }
  sf::Texture *comptexture;
};

class animation : public component
{
  sf::Texture **ownerTexture;
  std::vector<sf::Texture *> framesList;

public:
  float currentFrame;
  float frameTimeMult;
  static bool idAlreadyCreated;

  animation(gameObject *ownerObject, float frameTimeMultiplier = 1.0f);
  ~animation();
  void onUpdate();
  void onStart() {}
  const char *getCompName() { return "animation"; }
  void addAnimationFrame(sf::Texture *frame);
  void clearFrameList();
  //Frame num starts at 0
  sf::Texture  *getAnimationFrame(int frameNum);
  void setAnimationFrame(sf::Texture *frame, int frameNum);
};

#endif
