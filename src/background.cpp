#include <cstdlib>
#include "game_object.hpp"
#include "game.hpp"
#include "graphics_components.hpp"
#include "background.hpp"

void backgroundGeneration()
{
  //Planets
  for(int i = 0;i < 50;++i)
  {
    gameObject *planet = gameControl::createObject((rand() % 1920 - 1000) * 4, (rand() % 1080 - 500) * 4, 
      float(rand() % 19) / 20.0f + 0.05f);
    int rNum = rand() % 3;
    float planetSize = rand() % 300 + 112.0f;
    if(rNum == 0)
      planet->addComponent(new render("Planet_01", 0, 512, 0, 512, planetSize, planetSize));
    else if(rNum == 1)
      planet->addComponent(new render("Planet_02", 0, 512, 0, 512, planetSize, planetSize, 255, 255, 255, 255, 0.1f));
    else if(rNum == 2)
      planet->addComponent(new render("Planet_03", 0, 512, 0, 512, planetSize, planetSize));
  }
  //Stars
  for(int i = 0;i < 4000;++i)
  {
    gameObject *star = gameControl::createObject((rand() % 1920 - 1000) * 4, (rand() % 1080 - 500) * 4, 0.0f);
    float starSize = rand() % 22 + 2.0f;
    star->addComponent(new render("Star_04", 0, 32, 0, 32, starSize, starSize,
                        rand() % 255, rand() % 255, rand() % 255, rand() % 255, (rand() % 65 + 1) / 10.0f));

    //Add inside brightness of star
    gameObject *star2 = gameControl::createObject(star->getX(), star->getY(), 0.001f);
    star2->addComponent(new render("Star_04_Inside", 0, 32, 0, 32, starSize * 16, starSize * 16, 255 ,255 ,255 , 255, 0.1f));
  }
}