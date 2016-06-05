#ifndef SECTOR_HPP
#define SECTOR_HPP

#include "component.hpp"
#include <vector>

class sector : public component
{
public:
  int sectX, sectY;
  std::vector<gameObject *> objectsInSector;

  sector(int secX, int secY) : sectX(secX), sectY(secY) {}
  ~sector();
  void addToSector(gameObject *obj);
};

#endif
