#include <iostream>
#include <SFML/Graphics.hpp>

#include "eventHandler.hpp"

namespace cameraValues
{
  extern int scrollDelta;
}
namespace graphicsValues
{
  extern sf::RenderWindow *pwindow;
}
namespace statusValues
{
  extern bool gameIsRunning;
}

void handleEvents()
{
  sf::Event event;
  while(graphicsValues::pwindow->pollEvent(event))
  {
    switch(event.type)
    {
      case sf::Event::Closed:
        statusValues::gameIsRunning = 0;
        break;
      case sf::Event::MouseWheelMoved:
        cameraValues::scrollDelta = event.mouseWheel.delta;
        break;
      default:
        break;
    }
  }
}
