#include "initLogic.hpp"
#include "game_object.hpp"
#include "camera.hpp"
#include "background.hpp"
#include "graphics_components.hpp"
#include "ship_components.hpp"
#include "game.hpp"

#define SECTOR_SIZE 5000

namespace sectorMgmt
{
  extern bool genedSectors[7][7];
  extern int sectorSize;
  extern int centerX;
  extern int centerY;
  extern int prevCenterX;
  extern int prevCenterY;
}

namespace comPointers
{
  extern camera *pCamera;
}

namespace gameControl
{
  extern int worldSeed;
}

namespace selection
{
  gameObject *selectionBox;
}

void initSector()
{
  using namespace sectorMgmt;
  gameControl::worldSeed = 420;
  sectorSize = SECTOR_SIZE;
  for(int i = 0;i < 7;++i)
    for(int j = 0;j < 7;++j)genedSectors[i][j] = false;
  centerX = comPointers::pCamera->camX;
  centerY = comPointers::pCamera->camY;
  prevCenterX = centerX;
  prevCenterY = centerY;
}

//For putting test things in
void testInit()
{
  //Ship
  for(int i = 0;i < 5;++i)
    for(int j = 0;j < 5;++j)
    {
      gameObject *nship = gameControl::createObject((i - 2) * 500, (j - 2) * 500, 10.0f);
      nship->addComponent(new render("testship", 0, 512, 0, 512, 128, 128));
      nship->addComponent(new ship(100, 1.0f, 0.3f, 144, 0.04f, nship, 0.0f, 20.0f));
    }
}

void initMisc()
{
  //Selection box
  selection::selectionBox = gameControl::createObject(0, 0, 420.0f);
  selection::selectionBox->addComponent(new render("selectionBox", 0, 100, 0, 100, 100, 100, 255, 255, 255, 0));
}

void mainInit()
{
  initSector();
  initMisc();
  testInit();
}
