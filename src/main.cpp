#include "graphics_core.hpp"
#include "graphics_components.hpp"

namespace statusValues
{
  volatile int gameIsRunning = 1;
}

int main()
{
  //Init stuff
  initGraphics(1920, 1080, "Native Race");
  loadAllTextures();

  while(statusValues::gameIsRunning)
  {
      renderFrame();
      checkCloseWindow();
  }

  unloadGraphics();

  return 0;
}
