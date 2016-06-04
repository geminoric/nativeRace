#include "camera.hpp"
#include <SFML/Graphics.hpp>

#include <iostream>
#define DEFAULT_CAM_SPEED 10

bool camera::idAlreadyCreated = false;

namespace graphicsValues
{
  extern sf::RenderWindow *pwindow;
}
namespace statusValues
{
  extern int resX;
  extern int resY;
}

camera::camera(float x, float y, bool active) : camX(x), camY(y), isActive(active), camSpeed(DEFAULT_CAM_SPEED)
{
  //Increment the id 
  if(idAlreadyCreated)return;
  id = totalID++;
  idAlreadyCreated = true;
}

void camera::updateCameraPos()
{
   graphicsValues::pwindow->setView(sf::View(
    sf::FloatRect(camX - statusValues::resX / 2, camY - statusValues::resY / 2, 
                  statusValues::resX, statusValues::resY)));
}

void camera::onUpdate()
{
  //Check for movement input
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))camY -= camSpeed;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))camY += camSpeed;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))camX -= camSpeed;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))camX += camSpeed;
  if(isActive)updateCameraPos();
}
