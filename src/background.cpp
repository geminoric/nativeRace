#include <cstdlib>
#include <iostream>
#include <string.h>
#include <SFML/Graphics.hpp>
#include "game_object.hpp"
#include "game.hpp"
#include "graphics_components.hpp"
#include "background.hpp"
#include "structure_components.hpp"
#include "empire_components.hpp"
#include "collision.hpp"
#include "camera.hpp"
#include "sector.hpp"

#define PLANETS_PER_SECTOR 3
#define MAX_STARS_PER_SECTOR 15

#define PRIME_1 1004917
#define PRIME_2 1298113

namespace comPointers
{
  gameObject *startingStation;
  extern camera *pCamera;
}

namespace sectorMgmt
{
  bool genedSectors[7][7];
  int sectorSize;
  int centerX;
  int centerY;
  int prevCenterX;
  int prevCenterY;
}

namespace statusValues
{
  extern int resX;
  extern int resY;
}

namespace gameControl
{
  extern std::vector<gameObject *> gameObjects;
  std::vector<sector *> sectors;
  int worldSeed;
}

int getSectorSeed(int secX, int secY)
{
  return PRIME_1 * secX + PRIME_2 * secY * gameControl::worldSeed;
}

//X2, Y2 must be greater than X1, Y1
void generatePlanetsInArea(int x1, int y1, int x2, int y2, int num, sector *target)
{
  circleCollision *stationCol = comPointers::startingStation->getComponent<circleCollision>("circleCollision");
  for(int i = 0;i < num;++i)
  {
    float posX = (rand() % (x2 - x1) + x1);
    float posY = (rand() % (y2 - y1) + y1);

    gameObject *planet_ = gameControl::createObject(posX, posY, float(rand() % 19) / 20.0f + 0.05f);
    int rNum = rand() % 3;
    float planetSize = rand() % 200 + 200.0f;
    planet_->addComponent(new circleCollision(planetSize * 2, planet_));
    circleCollision *col = planet_->getComponent<circleCollision>("circleCollision");

    while(col->isColliding(stationCol))
    {
      posX = (rand() % (x2 - x1) + x1);
      posY = (rand() % (y2 - y1) + y1);
      planet_->x = posX;
      planet_->y = posY;
    }
    //Should have been planetSize / 2 in circleCollision constructor, didn't to make planets spawn further away
    col->radius /= 4;


    if(rNum == 0)
    {
      planet_->addComponent(new render("Planet_01", 0, 512, 0, 512, planetSize, planetSize,
        rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100, 255, rand() % 62 / 10.0f));
      planet_->addComponent(new planet(rand() % 32 / 10, rand() % 12 / 10, 0, 0, posX, posY, planetSize, target));
    }
    else if(rNum == 1)
    {
      planet_->addComponent(new render("Planet_02", 0, 512, 0, 512, planetSize, planetSize,
        rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100, 255, rand() % 62 / 10.0f));
      planet_->addComponent(new planet(rand() % 22 / 10, rand() % 16 / 10, rand() % 14 / 10, rand() % 11 / 10, posX, posY, planetSize, target));
    }
    else if(rNum == 2)
    {
      planet_->addComponent(new render("Planet_03", 0, 512, 0, 512, planetSize, planetSize,
        rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100, 255, rand() % 62 / 10.0f));
      planet_->addComponent(new planet(rand() % 22 / 10, rand() % 10 / 10, 0, rand() % 13 / 10, posX, posY, planetSize, target));
    }
    target->addToSector(planet_);
  }
}

void generateStarsInArea(int secX, int secY, sector *target)
{
  gameObject *starbase = gameControl::createObject(secX * sectorMgmt::sectorSize, secY * sectorMgmt::sectorSize, -0.001f);
  starbase->addComponent(new render("starField_Base", 0, 1024, 0, 1024, sectorMgmt::sectorSize, sectorMgmt::sectorSize));
  target->addToSector(starbase);

  for(int i = 0;i < rand() % MAX_STARS_PER_SECTOR + 3;++i)
  {
    int starSize = rand() % 64 + 50;
    gameObject *star = gameControl::createObject(rand() % sectorMgmt::sectorSize + secX * sectorMgmt::sectorSize, 
      rand() % sectorMgmt::sectorSize + secY * sectorMgmt::sectorSize, 0.0f);
    star->addComponent(new render("Star_04", 0, 32, 0, 32, starSize, starSize, rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100));
    target->addToSector(star);
    gameObject *star2 = gameControl::createObject(star->x, star->y, 0.0001f);
    star2->addComponent(new render("Star_04_Inside", 0, 32, 0, 32, starSize, starSize));
    target->addToSector(star2);
  }
}

void structureGeneration()
{
  srand(gameControl::worldSeed);
  gameObject *startStation = gameControl::createObject(rand() % 1800 - 900, rand() % 1000 - 500, 11.0f);
  startStation->addComponent(new render("Station_Base", 0, 512, 0, 512, 256, 256));
  startStation->addComponent(new circleCollision(256 / 2, startStation));
  comPointers::startingStation = startStation;
}

void generateSector(int secX, int secY)
{
  using sectorMgmt::sectorSize;

  srand(getSectorSeed(secX, secY));

  sector *pSector = new sector(secX, secY);
  gameControl::sectors.push_back(pSector);
  generatePlanetsInArea(secX * sectorSize, secY * sectorSize, secX * sectorSize + sectorSize, secY * sectorSize + sectorSize, PLANETS_PER_SECTOR, pSector);
  generateStarsInArea(secX, secY, pSector);
}

//If the camera centers somewhere else, moves array, also checks to delete sectors
void checkUpdateSecs()
{
  using namespace sectorMgmt;
  int difX = centerX / sectorSize - prevCenterX / sectorSize;
  int difY = centerY / sectorSize - prevCenterY / sectorSize;
  if(!difX && !difY)return;

  bool temp[7][7];

  for(int i = 0;i < 7;++i)
    for(int j = 0;j < 7;++j)
      temp[i][j] = false;

  //Shift over appropriately
  for(int i = 0;i < 7;++i)
    for(int j = 0;j < 7;++j)
    {
      //No parameter reorder
      if(i - difX >= 0 && i - difX < 7 && j - difY >= 0 && j - difY < 7 && sectorMgmt::genedSectors[i][j])
        temp[i - difX][j - difY] = true;
      else if(genedSectors[i][j])//Delete sector
      {
        for(std::vector<sector *>::iterator f = gameControl::sectors.begin();f != gameControl::sectors.end();++f)
        {
          if((*f) && (*f)->sectX == (prevCenterX / sectorSize) - 3 + i && (*f)->sectY == (prevCenterY / sectorSize) - 3 + j)
          {
            delete *f;
            *f = 0;
            f = gameControl::sectors.erase(f) - 1;
          }
        }
      }
    }

  for(int i = 0;i < 7;++i)
  {
    for(int j = 0;j < 7;++j)
    {
      genedSectors[i][j] = temp[i][j];
    }
  }

  prevCenterX = centerX;
  prevCenterY = centerY;
}

//Used a 7x7 bool array centered on player to store which sectors are generated
//Ungenerated ones at position 1 and 5 will be generated
//Sectors that leave array are degenerated
//Store objects in sector anchor
void sectorManagement()
{
  checkUpdateSecs();
  using namespace sectorMgmt;
  //Update center x and y
  centerX = int(comPointers::pCamera->camX) / sectorSize * sectorSize;
  centerY = int(comPointers::pCamera->camY) / sectorSize * sectorSize;

  int sectX = centerX / sectorSize;
  int sectY = centerY / sectorSize;
  for(int i = 1;i < 6;++i)
  {
    for(int j = 1;j < 6;++j)
    {
      //Check to generate a sector
      if(!genedSectors[i][j])
      {
        generateSector(sectX + i - 3, sectY + j - 3);
        genedSectors[i][j] = true;
      }
    }
  }
}
