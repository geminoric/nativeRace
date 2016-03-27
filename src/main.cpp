#include "graphics_core.hpp"
#include "graphics_components.hpp"

namespace StatusValues
{
  volatile int gameIsRunning = 1;
}

int main()
{
  //Init stuff
  initGraphics(1920, 1080, "Native Race");

  while(StatusValues::gameIsRunning)
  {
      renderFrame();
      checkCloseWindow();
  }

  unloadGraphics();

  return 0;
}
