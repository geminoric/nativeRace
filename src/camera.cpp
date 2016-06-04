#include "camera.hpp"
#include <SFML/Graphics.hpp>

#include <iostream>
#define DEFAULT_CAM_SPEED 10
#define ZOOM_RATE 0.2
#define MIN_ZOOM 0.4
#define MAX_ZOOM 3

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
namespace cameraValues
{
  int scrollDelta;
}

camera::camera(float x, float y, bool active) : camX(x), camY(y), isActive(active), camSpeed(DEFAULT_CAM_SPEED), zoom(1.0f)
{
  //Increment the id 
  if(idAlreadyCreated)return;
  id = totalID++;
  idAlreadyCreated = true;
}

void camera::updateCameraPos()
{
  sf::View view(
    sf::FloatRect(camX - statusValues::resX / 2, camY - statusValues::resY / 2, 
                  statusValues::resX, statusValues::resY));
  view.zoom(zoom);
  graphicsValues::pwindow->setView(view);
}

void camera::onUpdate()
{
  //Check for movement input
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))camY -= camSpeed;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))camY += camSpeed;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))camX -= camSpeed;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))camX += camSpeed;

  //Zooming in/out
  zoom -= cameraValues::scrollDelta * ZOOM_RATE;
  if(zoom > MAX_ZOOM)zoom = MAX_ZOOM;
  if(zoom < MIN_ZOOM)zoom = MIN_ZOOM;
  cameraValues::scrollDelta = 0;

  if(isActive)updateCameraPos();
}
