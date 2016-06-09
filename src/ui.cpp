#include <iostream>
#include "ui.hpp"
#include "empire_components.hpp"
#include "game_object.hpp"
#include "game.hpp"
#include "graphics_components.hpp"

namespace playerData
{
  resources *playerResources;
}

namespace UIStuff
{

}

void createResourcesUI()
{
  gameObject *res = gameControl::createObject(0, 0, 420.0f);
  res->addComponent(new resources(0, 0, 0, 0, 0));
  playerData::playerResources = res->getComponent<resources>("resources");
  playerData::playerResources->setColor(178, 102, 255);
}

void updateResourcesUI()
{
  //Reposition resource UI
}

void createUI()
{
  createResourcesUI();
}

void updateUI()
{
  updateResourcesUI();
}
