#ifndef EMPIRE_COMPONENTS_HPP
#define EMPIRE_COMPONENTS_HPP

#include "component.hpp"

class resources : public component
{
public:
  int ore, green, purple, orange;
  resources(int ore_, int green_, int purple_, int orange_) : ore(ore_), green(green_), purple(purple_), orange(orange_) {}
  resources() : ore(0), green(0), purple(0), orange(0) {}

  const char *getCompName() { return "resources"; }
};

#endif
