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
  //sizeX, sizeY unused
  float sizeX;
  float sizeY;
  float rot; //Radians
  int textX1, textX2, textY1, textY2;
  int textXSize, textYSize;
  int red, green, blue, alpha;
  sf::Texture *comptexture;

  //sizX, sizY (textXSize, textYSize) sets visible size
  render(int texX1, int texX2, int texY1, int texY2, float sizX, float sizY,
    int red_ = 255, int blue_ = 255, int green_ = 255, int alpha_ = 255, float rotRad = 0.0f);
  render(sf::Texture *texture, int texX1, int texX2, int texY1, int texY2, float sizX, float sizY,
    int red_ = 255, int blue_ = 255, int green_ = 255, int alpha_ = 255, float rotRad = 0.0f);
  render(const char *textName, int texX1, int texX2, int texY1, int texY2, float sizX, float sizY,
    int red_ = 255, int blue_ = 255, int green_ = 255, int alpha_ = 255, float rotRad = 0.0f);
  ~render();
  void setTexture(sf::Texture *texture) { comptexture = texture; }
  void onUpdate() {}
  void onStart() {}
  const char *getCompName() { return "render"; }
};

class animation : public component
{
  sf::Texture **ownerTexture;
  std::vector<sf::Texture *> framesList;

public:
  float currentFrame;
  float frameTimeMult;

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
