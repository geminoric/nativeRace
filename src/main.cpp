#include "graphics_core.hpp"
#include "graphics_components.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "background.hpp"
#include "camera.hpp"
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
  int resX = 1920;
  int resY = 1080;
}

namespace comPointers
{
  camera *pCamera;
}

int main()
{
  //Init stuff
  initGraphics(1920, 1080, "Native Race");
  loadAllTextures();
  srand(time(NULL));

  int fpsUpdateCountRes = 5;
  int fpsUpdateCountCur = 0;

  gameObject *cam = gameControl::createObject(0, 0, 0.0f);
  cam->addComponent(new camera(0.0f, 0.0f, true));
  backgroundGeneration();

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
        /*std::cout << "FPS: " <<
          1.0f / statusValues::deltaT
          << std::endl;*/
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
