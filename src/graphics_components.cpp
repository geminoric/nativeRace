#include "graphics_components.hpp"
#include "game_object.hpp"

bool render::idAlreadyCreated = false;
bool animation::idAlreadyCreated = false;

namespace statusValues
{
  extern float timeMultiplier;
}

render::render(float sizX, float sizY, float scalX, float scalY) : scaleX(scalX), scaleY(scalY), sizeX(sizX), sizeY(sizY)
{
  //Increment the id 
  if(idAlreadyCreated)return;
  id = totalID++;
  idAlreadyCreated = true;
}

render::render(sf::Texture *texture, float sizX, float sizY, float scalX, float scalY) : comptexture(texture), scaleX(scalX), scaleY(scalY), sizeX(sizX), sizeY(sizY)
{
  //Increment the id 
  if(idAlreadyCreated)return;
  id = totalID++;
  idAlreadyCreated = true;
}

render::render(const char *textName, float sizX, float sizY, float scalX, float scalY) : comptexture(findTexture(textName)), scaleX(scalX), scaleY(scalY), sizeX(sizX), sizeY(sizY)
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
