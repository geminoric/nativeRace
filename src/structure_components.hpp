#ifndef STRUCTURE_COMPONENTS_HPP
#define SRTUCTURE_COMPONENTS_HPP

#include "component.hpp"

class sector;

class planet : public component
{
public:
  //Resources
  int ore, purple, green, orange, ownerID;
  planet(int ore_, int green_, int purple_, int orange_, int posX, int posY, int planetSize, sector *target);

  const char *getCompName() { return "planet"; }
};

#endif
