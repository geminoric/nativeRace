#include "graphics_components.hpp"
#include "game_object.hpp"

bool render::idAlreadyCreated = false;
bool animation::idAlreadyCreated = false;

namespace statusValues
{
  extern float timeMultiplier;
}

render::render(int texX1, int texX2, int texY1, int texY2, float sizX, float sizY,
  float scalX, float scalY) : textX1(texX1), textX2(texX2), textY1(texY1), textY2(texY2), scaleX(scalX),
  scaleY(scalY), sizeX(sizX), sizeY(sizY), textXSize(texX2 - texX1), textYSize(texY2 - texY1)
{
  //Increment the id 
  if(idAlreadyCreated)return;
  id = totalID++;
  idAlreadyCreated = true;
}

render::render(sf::Texture *texture, int texX1, int texX2, int texY1, int texY2,
  float sizX, float sizY, float scalX, float scalY)
  : comptexture(texture), textX1(texX1), textX2(texX2), textY1(texY1), textY2(texY2), scaleX(scalX),
    scaleY(scalY), sizeX(sizX), sizeY(sizY), textXSize(texX2 - texX1), textYSize(texY2 - texY1)
{
  //Increment the id 
  if(idAlreadyCreated)return;
  id = totalID++;
  idAlreadyCreated = true;
}

render::render(const char *textName, int texX1, int texX2, int texY1, int texY2,
  float sizX, float sizY, float scalX, float scalY)
  : comptexture(findTexture(textName)), textX1(texX1), textX2(texX2), textY1(texY1), textY2(texY2),
    scaleX(scalX), scaleY(scalY), sizeX(sizX), sizeY(sizY), textXSize(texX2 - texX1), textYSize(texY2 - texY1)
{
  //Increment the id 
  if(idAlreadyCreated)return;
  id = totalID++;
  idAlreadyCreated = true;
}

render::~render()
{
  
}

animation::animation(gameObject *ownerObject, float frameTimeMultiplier) : frameTimeMult(frameTimeMultiplier), currentFrame(0.0f)
{
  owner = ownerObject;
  //Increment the id 
  if(idAlreadyCreated)return;
  id = totalID++;
  idAlreadyCreated = true;
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
