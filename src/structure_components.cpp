#include <iostream>
#include "structure_components.hpp"
#include "game_object.hpp"
#include "game.hpp"
#include "graphics_components.hpp"
#include "empire_components.hpp"

#define ICON_SIZE 48

namespace sectorMgmt
{
  extern int sectorSize;
}

planet::planet(int ore_, int green_, int purple_, int orange_, int posX, int posY, int planetSize) : ore(ore_), green(green_), purple(purple_), orange(orange_)
{
  int numRes = !!ore + !!green + !!orange + !!purple;
  //Create resource icons
  //Starting position(-0.5 * ICON_SIZE * numRes) + ICON_SIZE * icon distance
  for(int i = 0;i < numRes;++i)
  {
    if(ore_)
    {
      gameObject *res = gameControl::createObject(posX + planetSize / 2 + (-ICON_SIZE / 2 * numRes) + ICON_SIZE * i, posY + planetSize / 2 + 0.0f, 7.0f);
      res->addComponent(new render("resource_ore", 0, 64, 0, 64, ICON_SIZE, ICON_SIZE));
      res->addComponent(new cleanable((int)posX / sectorMgmt::sectorSize * sectorMgmt::sectorSize, (int)posY / sectorMgmt::sectorSize * sectorMgmt::sectorSize));
      ore_ = 0;
    }
    else if(green_)
    {
      gameObject *res = gameControl::createObject(posX + planetSize / 2 + (-ICON_SIZE / 2 * numRes) + ICON_SIZE * i, posY + planetSize / 2 + 0.0f, 7.0f);
      res->addComponent(new render("resource_green", 0, 64, 0, 64, ICON_SIZE, ICON_SIZE));
      res->addComponent(new cleanable((int)posX / sectorMgmt::sectorSize * sectorMgmt::sectorSize, (int)posY / sectorMgmt::sectorSize * sectorMgmt::sectorSize));
      green_ = 0;
    }
    else if(purple_)
    {
      gameObject *res = gameControl::createObject(posX + planetSize / 2 + (-ICON_SIZE / 2 * numRes) + ICON_SIZE * i, posY + planetSize / 2 + 0.0f, 7.0f);
      res->addComponent(new render("resource_purple", 0, 64, 0, 64, ICON_SIZE, ICON_SIZE));
      res->addComponent(new cleanable((int)posX / sectorMgmt::sectorSize * sectorMgmt::sectorSize, (int)posY / sectorMgmt::sectorSize * sectorMgmt::sectorSize));
      purple_ = 0;
    }
    else if(orange_)
    {
      gameObject *res = gameControl::createObject(posX + planetSize / 2 + (-ICON_SIZE / 2 * numRes) + ICON_SIZE * i, posY + planetSize / 2 + 0.0f, 7.0f);
      res->addComponent(new render("resource_orange", 0, 64, 0, 64, ICON_SIZE, ICON_SIZE));
      res->addComponent(new cleanable((int)posX / sectorMgmt::sectorSize * sectorMgmt::sectorSize, (int)posY / sectorMgmt::sectorSize * sectorMgmt::sectorSize));
      orange_ = 0;
    }
  }
}
