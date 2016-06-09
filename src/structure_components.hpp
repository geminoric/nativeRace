#ifndef STRUCTURE_COMPONENTS_HPP
#define SRTUCTURE_COMPONENTS_HPP

#include "component.hpp"

class sector;
class resources;
class render;

class planet : public component
{
  gameObject *owner;
  gameObject *pCapOwner;
  gameObject *pSovOwner;
  render *pCapCircle;
  render *pSovCircle;
  bool capThisFrame;

  void updateCaptureColor();
public:
  int colR, colG, colB, capProgress, capMax, planetSize;
  //Resources
  int ore, purple, green, orange, ownerID;
  planet(int ore_, int green_, int purple_, int orange_, int posX, int posY, int planetSize_, sector *target, gameObject *parent, int maxCap = 300);
  ~planet();
  void capture(int facID, resources *facRes);
  void deCapture(int facID);

  void onUpdate();
  const char *getCompName() { return "planet"; }
};

struct savedPlanet
{
  float x, y;
  int ownerFacID;
  resources *ownerRes;
  savedPlanet(float posX, float posY, int ownerID, resources *ownerResource) : x(posX), y(posY), ownerFacID(ownerID), ownerRes(ownerResource) {}
};

#endif
