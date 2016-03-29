#include "graphics_core.hpp"
#include "graphics_components.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include <vector>
#include <memory>

namespace statusValues
{
  volatile int gameIsRunning = 1;
}

int main()
{
  //Init stuff
  initGraphics(1920, 1080, "Native Race");
  loadAllTextures();
  //Test create object
  gameObject *test = gameControl::createObject(200,200,0.0f);
  test->components.push_back(std::shared_ptr<component>(new render("testship.png")));

  while(statusValues::gameIsRunning)
  {
      renderFrame(gameControl::gameObjects);
      checkCloseWindow();
  }

  gameControl::deleteAllObjects();
  unloadGraphics();

  return 0;
}
