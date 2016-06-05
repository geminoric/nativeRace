#include <cstdlib>
#include <iostream>
#include "game_object.hpp"
#include "game.hpp"
#include "graphics_components.hpp"
#include "background.hpp"
#include "structure_components.hpp"
#include "empire_components.hpp"
#include "collision.hpp"
#include "camera.hpp"
#include "sector.hpp"

#define PLANETS_PER_SECTOR 1
#define STARS_PER_SECTOR 1

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

    planet_->addComponent(new cleanable((int)posX / sectorMgmt::sectorSize * sectorMgmt::sectorSize, (int)posY / sectorMgmt::sectorSize * sectorMgmt::sectorSize));

    if(rNum == 0)
    {
      planet_->addComponent(new render("Planet_01", 0, 512, 0, 512, planetSize, planetSize,
        rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100, 255, rand() % 62 / 10.0f));
      planet_->addComponent(new planet(rand() % 32 / 10, rand() % 12 / 10, 0, 0, posX, posY, planetSize));
    }
    else if(rNum == 1)
    {
      planet_->addComponent(new render("Planet_02", 0, 512, 0, 512, planetSize, planetSize,
        rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100, 255, rand() % 62 / 10.0f));
      planet_->addComponent(new planet(rand() % 22 / 10, rand() % 16 / 10, rand() % 14 / 10, rand() % 11 / 10, posX, posY, planetSize));
    }
    else if(rNum == 2)
    {
      planet_->addComponent(new render("Planet_03", 0, 512, 0, 512, planetSize, planetSize,
        rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100, 255, rand() % 62 / 10.0f));
      planet_->addComponent(new planet(rand() % 22 / 10, rand() % 10 / 10, 0, rand() % 13 / 10, posX, posY, planetSize));
    }
    target->addToSector(planet_);
  }
}

void generateStarsInArea(int x1, int y1, int x2, int y2, int num, sector *target)
{
  for(int i = 0;i < num;++i)
  {
    gameObject *star = gameControl::createObject((rand() % (x2 - x1) + x1), (rand() % (y2 - y1) + y1), 0.0f);
    float starSize = rand() % 22 + 2.0f;
    star->addComponent(new cleanable((int)star->x / sectorMgmt::sectorSize * sectorMgmt::sectorSize, (int)star->y / sectorMgmt::sectorSize * sectorMgmt::sectorSize));
    star->addComponent(new render("Star_04", 0, 32, 0, 32, starSize, starSize,
                        rand() % 255, rand() % 255, rand() % 255, rand() % 255, (rand() % 65 + 1) / 10.0f));

    //Add inside brightness of star
    gameObject *star2 = gameControl::createObject(star->getX(), star->getY(), 0.001f);
    star2->addComponent(new render("Star_04_Inside", 0, 32, 0, 32, starSize, starSize, 255 ,255 ,255 , 255, 0.1f));
    star2->addComponent(new cleanable((int)star->x / sectorMgmt::sectorSize * sectorMgmt::sectorSize, (int)star->y / sectorMgmt::sectorSize * sectorMgmt::sectorSize));
    
    target->addToSector(star);
    target->addToSector(star2);
  }
}

void structureGeneration()
{
  gameObject *startStation = gameControl::createObject(rand() % 1800 - 900, rand() % 1000 - 500, 11.0f);
  startStation->addComponent(new render("Station_Base", 0, 512, 0, 512, 256, 256));
  startStation->addComponent(new circleCollision(256 / 2, startStation));
  comPointers::startingStation = startStation;
}

void generateSector(int secX, int secY)
{
  sector *pSector = new sector(secX, secY);
  gameControl::sectors.push_back(pSector);
  generatePlanetsInArea(x1, y1, x2, y2, PLANETS_PER_SECTOR, pSector);
  generateStarsInArea(x1, y1, x2, y2, STARS_PER_SECTOR, pSector);
}

//If the camera centers somewhere else, moves array, also checks to delete sectors
void checkUpdateSecs()
{
  using namespace sectorMgmt;
  int difX = (centerX - prevCenterX) / sectorMgmt::sectorSize;
  int difY = (centerY - prevCenterY) / sectorMgmt::sectorSize;
  bool temp[7][7];

  for(int i = 0;i < 7;++i)
    for(int j = 0;j < 7;++j)
      temp[i][j] = false;

  //Shift over appropriately
  for(int i = 0;i < 7;++i)
    for(int j = 0;j < 7;++j)
    {
      //No parameter reorder
      if(i + difX >= 0 && i + difX < 7 && j + difY >= 0 && j + difY < 7 && sectorMgmt::genedSectors[i][j])
        temp[i + difX][j + difY] = true;
      else if(genedSectors[i][j])//Delete sector
      {
        for(std::vector<gameObject *>::iterator f = gameControl::gameObjects.begin();f != gameControl::gameObjects.end();++f)
        {
          cleanable *pClean = (*f)->getComponent<cleanable>("cleanable");
          //if(pClean)std::cout << pClean->centerX << ", " << pClean->centerY << " || " << centerX - (i - 3) * sectorSize << ", " << centerY - (j - 3) * sectorSize << " || " << difX << ", " << difY << std::endl;
          if(pClean && pClean->centerX == centerX - (i - 3) * sectorSize && centerY - pClean->centerY == (j - 3) * sectorSize)
          {
            std::cout << "D ";
            gameControl::deleteObject(*f);
          }
        }
        std::cout << std::endl;
        genedSectors[i][j] = false;
      }
    }

  for(int i = 0;i < 7;++i)
    for(int j = 0;j < 7;++j)
      genedSectors[i][j] = temp[i][j];

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
  centerX = int(comPointers::pCamera->camX + statusValues::resX / 2) / sectorSize * sectorSize;
  centerY = int(comPointers::pCamera->camY + statusValues::resY / 2) / sectorSize * sectorSize;

  int sectX = centerX / sectorSize;
  int sectY = centerY / sectorSize;
  for(int i = 0;i < 7;++i)
  {
    for(int j = 0;j < 7;++j)
    {
      //Check to generate a sector
      //Keep the order of parameters - short circuit
      if(i > 0 && i < 6 && j > 0 && j < 6 && !genedSectors[i][j])
      {
        generateSector(sectX + i - 3, sectY + j - 3);
        genedSectors[i][j] = true;
      }
    }
  }
}
