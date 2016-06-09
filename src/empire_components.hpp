#ifndef EMPIRE_COMPONENTS_HPP
#define EMPIRE_COMPONENTS_HPP

#include "component.hpp"

class resources : public component
{
public:
  int ore, green, purple, orange;
  //Player factionID is 0
  int factionID;
  //Faction color
  int redCol, greenCol, blueCol;
  resources(int ore_, int green_, int purple_, int orange_, int facID) : ore(ore_), green(green_), purple(purple_), orange(orange_), factionID(facID) {}
  resources() : ore(0), green(0), purple(0), orange(0) {}
  void setColor(int r, int g, int b) { redCol = r; greenCol = g; blueCol = b; }

  const char *getCompName() { return "resources"; }
};

#endif
