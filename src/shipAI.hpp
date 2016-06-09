#ifndef SHIPAI_HPP
#define SHIPAI_HPP

#include "component.hpp"

class planet;
class ship;

class shipAI : public component
{
  gameObject *owner;
  ship *pShip;
  planet *targPlanet;
  int curUpdate, maxUpdate;
public:
  shipAI(gameObject *parent, ship *ownerShip) : owner(parent), pShip(ownerShip), targPlanet(0), curUpdate(100), maxUpdate(100) {}

  void onUpdate();

  const char *getCompName() { return "shipAI"; }
};

#endif
