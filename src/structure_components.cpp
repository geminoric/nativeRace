#include <iostream>
#include <vector>
#include "structure_components.hpp"
#include "game_object.hpp"
#include "game.hpp"
#include "graphics_components.hpp"
#include "empire_components.hpp"
#include "sector.hpp"

#define ICON_SIZE 48

namespace sectorMgmt
{
  extern int sectorSize;
  std::vector<savedPlanet> savedPlanets;
}

namespace playerData
{
  extern resources *playerResources;
}

planet::planet(int ore_, int green_, int purple_, int orange_, int posX, int posY, int planetSize_, sector *target, gameObject *parent, int maxCap) : 
  ore(ore_), green(green_), purple(purple_), orange(orange_), ownerID(-1), owner(parent), capMax(maxCap), colR(0), colG(0), colB(0),
  capThisFrame(false), capProgress(0), planetSize(planetSize_)
{
  //Create capture circle
  pCapOwner = gameControl::createObject(owner->x + planetSize / 2, owner->y + planetSize / 2, owner->zOrder + 0.001f);
  pCapOwner->addComponent(new render("capCircle", 0, 256, 0, 256, planetSize, planetSize, 255, 255, 255, 0));
  pCapCircle = pCapOwner->getComponent<render>("render");
  //Create sov circle
  pSovOwner = gameControl::createObject(owner->x - planetSize * 3.5, owner->y - planetSize * 3.5, 42.123f);
  pSovOwner->addComponent(new render("capCircle", 0, 256, 0, 256, planetSize * 8, planetSize * 8, 255, 255, 255, 0));
  pSovCircle = pSovOwner->getComponent<render>("render");

  int numRes = !!ore + !!green + !!orange + !!purple;
  //Create resource icons
  //Starting position(-0.5 * ICON_SIZE * numRes) + ICON_SIZE * icon distance
  for(int i = 0;i < numRes;++i)
  {
    if(ore_)
    {
      gameObject *res = gameControl::createObject(posX + planetSize / 2 + (-ICON_SIZE / 2 * numRes) + ICON_SIZE * i, posY + planetSize / 2 + 0.0f, 7.0f);
      res->addComponent(new render("resource_ore", 0, 64, 0, 64, ICON_SIZE, ICON_SIZE));
      target->addToSector(res);
      ore_ = 0;
    }
    else if(green_)
    {
      gameObject *res = gameControl::createObject(posX + planetSize / 2 + (-ICON_SIZE / 2 * numRes) + ICON_SIZE * i, posY + planetSize / 2 + 0.0f, 7.0f);
      res->addComponent(new render("resource_green", 0, 64, 0, 64, ICON_SIZE, ICON_SIZE));
      target->addToSector(res);
      green_ = 0;
    }
    else if(purple_)
    {
      gameObject *res = gameControl::createObject(posX + planetSize / 2 + (-ICON_SIZE / 2 * numRes) + ICON_SIZE * i, posY + planetSize / 2 + 0.0f, 7.0f);
      res->addComponent(new render("resource_purple", 0, 64, 0, 64, ICON_SIZE, ICON_SIZE));
      target->addToSector(res);
      purple_ = 0;
    }
    else if(orange_)
    {
      gameObject *res = gameControl::createObject(posX + planetSize / 2 + (-ICON_SIZE / 2 * numRes) + ICON_SIZE * i, posY + planetSize / 2 + 0.0f, 7.0f);
      res->addComponent(new render("resource_orange", 0, 64, 0, 64, ICON_SIZE, ICON_SIZE));
      target->addToSector(res);
      orange_ = 0;
    }
  }
  //If planet has already been captured load data and then delete the save
  for(std::vector<savedPlanet>::iterator i = sectorMgmt::savedPlanets.begin(); i != sectorMgmt::savedPlanets.end();++i)
  {
    if(owner->x == i->x && owner->y == i->y)
    {
      capProgress = capMax;
      ownerID = i->ownerFacID;
      colR = i->ownerRes->redCol;
      colG = i->ownerRes->greenCol;
      colB = i->ownerRes->blueCol;
      sectorMgmt::savedPlanets.erase(i);
      break;
    }
  }
}

planet::~planet()
{
  //Save planet's owner
  if(capProgress >= capMax)
    sectorMgmt::savedPlanets.push_back(savedPlanet(owner->x, owner->y, ownerID, playerData::playerResources));
  gameControl::deleteObject(pCapOwner);
  gameControl::deleteObject(pSovOwner);
}

//Fix to work with more than the player faction
void planet::capture(int facID, resources *facRes)
{
  if(capThisFrame || (capProgress >= capMax && facID == ownerID))return;

  ++capProgress;
  colR = facRes->redCol;
  colG = facRes->greenCol;
  colB = facRes->blueCol;
  if(capProgress >= capMax)
  {
    ownerID = facID; 
  }

  capThisFrame = true;
}

void planet::deCapture(int facID)
{
  if(facID == ownerID)return;
  --capProgress;
  if(capProgress < 0)capProgress = 0;
}

void planet::updateCaptureColor()
{
  //Reset cap if no longer capping
  if(!capThisFrame && capProgress < capMax)
    {
      capProgress -= 5;
      if(capProgress < 0)capProgress = 0;
    }
  if(capProgress <= 0 || capProgress >= capMax)
  {
    pCapCircle->alpha = 0;
    //Sov circle
    if(capProgress >= capMax)
    {
      pSovCircle->red = colR;
      pSovCircle->green = colG;
      pSovCircle->blue = colB;
      pSovCircle->alpha = 63;
    }
    return;
  }
  pSovCircle->alpha = 0;
  //Capture circle
  float capRatio = float(capProgress) / capMax;

  pCapOwner->x = owner->x + planetSize / 2 * (1.0 - capRatio);
  pCapOwner->y = owner->y + planetSize / 2 * (1.0 - capRatio);

  pCapCircle->alpha = 255;
  pCapCircle->red = colR;
  pCapCircle->green = colG;
  pCapCircle->blue = colB;
  pCapCircle->textXSize = planetSize * capRatio;
  pCapCircle->textYSize = planetSize * capRatio;
}

void planet::onUpdate()
{
  updateCaptureColor();
  capThisFrame = false;
}
