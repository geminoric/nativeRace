#include "initLogic.hpp"
#include "game_object.hpp"
#include "camera.hpp"
#include "background.hpp"
#include "graphics_components.hpp"
#include "ship_components.hpp"
#include "game.hpp"
#include "shipAI.hpp"

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

namespace temp
{
  int enemyCount;
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
  temp::enemyCount = 0;
  //Ship
  for(int i = 0;i < 5;++i)
    for(int j = 0;j < 5;++j)
    {
      gameObject *nship = gameControl::createObject((i - 2) * 500, (j - 2) * 500, 30.0f);
      nship->addComponent(new render("testship", 0, 512, 0, 512, 128, 128));
      nship->addComponent(new ship(100, 1.0f, 0.04f, 144, 0.04f, nship, 0.0f, 30.0f));
    }
}

//Spawn enemy ships offscreen - TEMP
void tempCheckSpawnEnemies()
{
  if(rand() % 1000 > 990 && temp::enemyCount < 10)
  {
    for(int i = 0;i < 2;++i)
    {
      for(int j = 0;j < 2;++j)
      {
        float x = 5000.0f + rand() % 2500;
        float y = 5000.0f + rand() % 2500;
        if(rand() % 2)x *= -1;
        if(rand() % 2)y *= -1;

        gameObject *nship = gameControl::createObject(comPointers::pCamera->camX + x, comPointers::pCamera->camY + y, 30.0f);
        nship->addComponent(new render("bombDroneOn", 0, 512, 0, 512, 128, 128));
        nship->addComponent(new ship(50, 1.0f, 0.04f, 144, 0.04f, nship, 0.0f, 30.0f, 1));
        nship->addComponent(new shipAI(nship, nship->getComponent<ship>("ship")));
        temp::enemyCount++;
      }
    }
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
