#include "graphics_core.hpp"
#include "graphics_components.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include <vector>
#include <memory>
#include <iostream>
#include <ctime>
#include <chrono>

const int MAX_FRAMERATE_F = 60.0f;

namespace statusValues
{
  volatile int gameIsRunning = 1;
  //Average change in time per second over past 5(fpsUpdateCountRes) frames
  float deltaT = 0.01;
  // 1/framerate * deltaT
  float timeMultiplier = 1.0;
}

int main()
{
  //Init stuff
  initGraphics(1920, 1080, "Native Race");
  loadAllTextures();
  srand(time(NULL));

  int fpsUpdateCountRes = 5;
  int fpsUpdateCountCur = 0;

  //Test animation stuff
  gameObject *test = gameControl::createObject(300.0f, 200.0f, 0.0f);
  test->addComponent(new render("playerDownSprite1", 16.0f, 16.0f, 8.0f, 8.0f));
  test->addComponent(new animation(test, 0.2f));
  animation *animtest = test->getComponent<animation>("animation");
  animtest->addAnimationFrame(findTexture("playerDownSprite1"));
  animtest->addAnimationFrame(findTexture("playerDownSprite2"));
  animtest->addAnimationFrame(findTexture("playerDownSprite3"));
  animtest->addAnimationFrame(findTexture("playerDownSprite4"));
  animtest->addAnimationFrame(findTexture("playerDownSprite5"));

  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

  while(statusValues::gameIsRunning)
  {
      renderFrame(gameControl::gameObjects);
      checkCloseWindow();

      gameControl::runObjectUpdate();

      //Update framerate
      ++fpsUpdateCountCur;
      if(fpsUpdateCountCur >= fpsUpdateCountRes)
      {
        //Print framerate
        statusValues::deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / (1000.0f * fpsUpdateCountRes);
        std::cout << "FPS: " <<
          1.0f / statusValues::deltaT
          << std::endl;
        //Reset frame timer
        start = std::chrono::steady_clock::now();
        fpsUpdateCountCur = 0;
        statusValues::timeMultiplier = statusValues::deltaT * 1.0f * MAX_FRAMERATE_F;
      }
  }

  gameControl::deleteAllObjects();
  unloadGraphics();

  return 0;
}
