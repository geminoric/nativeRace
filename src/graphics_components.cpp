#include "graphics_components.hpp"
#include "game_object.hpp"
#include <iostream>

namespace statusValues
{
  extern float timeMultiplier;
}

render::render(int texX1, int texX2, int texY1, int texY2, float sizX, float sizY,
  int red_, int blue_, int green_, int alpha_, float rotRad) :
  textX1(texX1), textX2(texX2), textY1(texY1), textY2(texY2), 
  textXSize(sizX), textYSize(sizY),
  red(red_), green(green_), blue(blue_), alpha(alpha_), rot(rotRad)
{
}

render::render(sf::Texture *texture, int texX1, int texX2, int texY1, int texY2,
  float sizX, float sizY, int red_, int blue_, int green_, int alpha_, float rotRad)
  : comptexture(texture), textX1(texX1), textX2(texX2), textY1(texY1), textY2(texY2), 
  textXSize(sizX), textYSize(sizY),
  red(red_), green(green_), blue(blue_), alpha(alpha_), rot(rotRad)
{
}

render::render(const char *textName, int texX1, int texX2, int texY1, int texY2,
  float sizX, float sizY, int red_, int blue_, int green_, int alpha_, float rotRad)
  : comptexture(findTexture(textName)), textX1(texX1), textX2(texX2), textY1(texY1), textY2(texY2),
    textXSize(sizX), textYSize(sizY),
  red(red_), green(green_), blue(blue_), alpha(alpha_), rot(rotRad)
{
}

render::~render()
{
  
}

animation::animation(gameObject *ownerObject, float frameTimeMultiplier) : frameTimeMult(frameTimeMultiplier), currentFrame(0.0f)
{
  owner = ownerObject;
  //Set up animation owner's texture pointer
  ownerTexture = &(owner->getComponent<render>("render")->comptexture);
}

animation::~animation()
{

}

//Increment current frame and update frame
void animation::onUpdate()
{
  currentFrame += frameTimeMult * statusValues::timeMultiplier;
  if(currentFrame > framesList.size() - 1)currentFrame = 0.0f;

  *ownerTexture = framesList[static_cast<int>(currentFrame)];
}

void animation::addAnimationFrame(sf::Texture *frame)
{
  framesList.push_back(frame);
}

void animation::clearFrameList()
{
  framesList.clear();
}

sf::Texture *animation::getAnimationFrame(int frameNum)
{
  return framesList[frameNum];
}

void animation::setAnimationFrame(sf::Texture *frame, int framenum)
{
  framesList[framenum] = frame;
}
