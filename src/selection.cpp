#include <iostream>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include "ship_components.hpp"
#include "graphics_components.hpp"
#include "game_object.hpp"
#include "selection.hpp"
#include "camera.hpp"

namespace statusValues
{
  //Mouse position relative to space
  int mouseX;
  int mouseY;
  int windowX;
  int windowY;
  extern int resX;
  extern int resY;
}
namespace comPointers
{
  extern camera *pCamera;
}
namespace graphicsValues
{
  extern sf::RenderWindow *pwindow;
}
namespace gameControl
{
  extern std::vector<gameObject *> gameObjects;
}
namespace selection
{
  bool boxActive;
  int x1, x2, y1, y2;
  std::vector<ship *> shipsInBox;
  extern gameObject *selectionBox;
}

void updateMousePos()
{
  auto pos = sf::Mouse::getPosition(*graphicsValues::pwindow);
  statusValues::windowX = graphicsValues::pwindow->getSize().x;
  statusValues::windowY = graphicsValues::pwindow->getSize().y;

  
  pos.x = (float)pos.x / statusValues::windowX * statusValues::resX;
  pos.y = (float)pos.y / statusValues::windowY * statusValues::resY;
  statusValues::mouseX = comPointers::pCamera->camX + comPointers::pCamera->zoom * (pos.x - statusValues::resX / 2);
  statusValues::mouseY = comPointers::pCamera->camY + comPointers::pCamera->zoom  * (pos.y - statusValues::resY / 2);
}

void updateBoxGraphic()
{
  render *pRender = selection::selectionBox->getComponent<render>("render");
  pRender->alpha = 255 * selection::boxActive;
  if(!selection::boxActive)return;

  if(selection::x1 < selection::x2)
  {
    selection::selectionBox->x = selection::x1;
    pRender->textXSize = selection::x2 - selection::x1;
  }
  else
  {
    selection::selectionBox->x = selection::x2;
    pRender->textXSize = selection::x1 - selection::x2;
  }

  if(selection::y1 < selection::y2)
  {
    selection::selectionBox->y = selection::y1;
    pRender->textYSize = selection::y2 - selection::y1;
  }
  else
  {
    selection::selectionBox->y = selection::y2;
    pRender->textYSize = selection::y1 - selection::y2;
  }
}

//Returns true if colliding
//Note : ignores rotation
bool checkBoxCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
  if(x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2)
    return true;
  return false;
}

//Refresh the objects in the selection box
void updateObjectsInBox()
{
  //Clear current selection
  for(std::vector<ship *>::iterator i = selection::shipsInBox.begin();i != selection::shipsInBox.end();++i)
  {
    //Remove selected flag from ships
    (*i)->setSelected(false);
  }
  selection::shipsInBox.clear();

  int boxWidth = selection::x2 - selection::x1;
  int boxHeight = selection::y2 - selection::y1;
  if(boxWidth < 0)boxWidth *= -1;
  if(boxHeight < 0)boxHeight *= -1;

  for(std::vector<gameObject *>::iterator i = gameControl::gameObjects.begin();i != gameControl::gameObjects.end();++i)
  {
    ship *pShip;
    if(!(pShip = (*i)->getComponent<ship>("ship")))continue;
    render *pRender = (*i)->getComponent<render>("render");

    //Temp values for start of box collider
    int tX1 = selection::x1;
    int tY1 = selection::y1;
    if(selection::x1 > selection::x2)tX1 = selection::x2;
    if(selection::y1 > selection::y2)tY1 = selection::y2;

    //Check if each object is colliding with box, if it is add it to selection and make it selected
    if(checkBoxCollision(tX1, tY1, boxWidth, boxHeight, (*i)->x, (*i)->y, pRender->textXSize, pRender->textYSize))
    {
      pShip->setSelected(true);
      selection::shipsInBox.push_back(pShip);
    }
  }
}

void updateSelectionBox()
{
  using namespace selection;
  if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    if(boxActive)//Update x2, y2
    {
      selection::x2 = statusValues::mouseX;
      selection::y2 = statusValues::mouseY;
    }
    else//Create a new selection box
    {
      boxActive = true;
      selection::x1 = statusValues::mouseX;
      selection::y1 = statusValues::mouseY;
      selection::x2 = statusValues::mouseX;
      selection::y2 = statusValues::mouseY;
    }
  }//Close box and update selection list
  else if(boxActive)
  {
    updateObjectsInBox();
    boxActive = false;
  }
  updateBoxGraphic();
}

void checkMoveUnits()
{
  if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
  {
    selection::boxActive = false;
    for(std::vector<ship *>::iterator i = selection::shipsInBox.begin();i != selection::shipsInBox.end();++i)
    {
      (*i)->targX = statusValues::mouseX;
      (*i)->targY = statusValues::mouseY;
      //Remove selected flag from ships
      (*i)->setSelected(false);
    }
    //Clear selection after moving
    selection::shipsInBox.clear();
  }
}

void manageSelection()
{
  updateMousePos();
  updateSelectionBox();
}

void manageRTSControls()
{
  checkMoveUnits();
}
