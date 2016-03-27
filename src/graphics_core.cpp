#include <SFML/Graphics.hpp>
#include "graphics_core.hpp"

namespace
{
  sf::RenderWindow *pwindow;
}

namespace StatusValues
{
  extern volatile int gameIsRunning;
}

//Returns 0 if no error
int initGraphics(int resX, int resY, const char *windowTitle)
{
  pwindow = new sf::RenderWindow(sf::VideoMode(resX, resY), windowTitle);
  return 0;
}

//Returns 0 if no error
int renderFrame()
{
  pwindow->clear();
  pwindow->display();
  return 0;
}

//Call this every frame to check if the window needs to be closed
void checkCloseWindow()
{
  sf::Event event;
  while(pwindow->pollEvent(event))
  {
    if(event.type == sf::Event::Closed)
        StatusValues::gameIsRunning = 0;
  }
}

//Destroys the window, as well as all other graphics components
void unloadGraphics()
{
  pwindow->close();
  delete pwindow;
}

