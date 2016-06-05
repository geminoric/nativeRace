#include "initLogic.hpp"
#include "game_object.hpp"
#include "camera.hpp"

#define SECTOR_SIZE 1000

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

void initSector()
{
  using namespace sectorMgmt;
  sectorSize = SECTOR_SIZE;
  for(int i = 0;i < 7;++i)
    for(int j = 0;j < 7;++j)genedSectors[i][j] = false;
  centerX = comPointers::pCamera->camX;
  centerY = comPointers::pCamera->camY;
  prevCenterX = centerX;
  prevCenterY = centerY;
}

void mainInit()
{
  initSector();
}
