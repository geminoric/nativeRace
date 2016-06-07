#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <cmath>
#include "graphics_core.hpp"
#include "graphics_components.hpp"
#include "game.hpp"
#include "game_object.hpp"

const int MAX_FRAMERATE = 60;

namespace graphicsValues
{
  sf::RenderWindow *pwindow;
}

namespace statusValues
{
  extern volatile int gameIsRunning;
}

namespace loadedTextures
{
  std::vector<std::tuple<std::string, sf::Texture, sf::Vector2f>> textures;
}

//Returns 0 if no error
int initGraphics(int resX, int resY, const char *windowTitle)
{
  graphicsValues::pwindow = new sf::RenderWindow(sf::VideoMode(resX, resY), windowTitle);

  sf::View view(sf::FloatRect(0, 0, 1920, 1080));
  graphicsValues::pwindow->setView(view);

  graphicsValues::pwindow->setFramerateLimit(MAX_FRAMERATE);
  return 0;
}

void loadTexture(const char *loc, const char *name, int *currentret, int xSize, int ySize)
{
  sf::Texture ntext;
  if(!ntext.loadFromFile(loc))
  {
    std::cout << "[ERROR] Could not load " << loc << std::endl;
    if(currentret != 0)++(*currentret);
  }
  else
  {
    loadedTextures::textures.push_back(std::tuple<std::string, sf::Texture, sf::Vector2f>(name, ntext, sf::Vector2f(xSize, ySize)));
  }
}

void loadTextureFromSpriteSheet(const char *loc, const char *name, int *currentret,
                                int xSize, int ySize, int xLeftSpriteSheet, int yTopSpriteSheet)
{
  sf::Texture ntext;
  if(!ntext.loadFromFile(loc, sf::Rect<int>(xLeftSpriteSheet, yTopSpriteSheet, xSize, ySize)))
  {
    std::cout << "[ERROR] Could not load " << loc << std::endl;
    if(currentret != 0)++(*currentret);
  }
  else
  {
    loadedTextures::textures.push_back(std::tuple<std::string, sf::Texture, sf::Vector2f>(name, ntext, sf::Vector2f(xSize, ySize)));
  }
}

//Returns the amount of errors in loading textures
int loadAllTextures()
{
  int ret = 0;
  
  loadTexture("../images/BombDrone_On.png", "bombDroneOn", &ret, 512, 512);
  loadTexture("../images/testship.png", "testship", &ret, 512, 512);
  loadTexture("../images/Planet_01.png", "Planet_01", &ret, 512, 512);
  loadTexture("../images/Planet_02.png", "Planet_02", &ret, 512, 512);
  loadTexture("../images/Planet_03.png", "Planet_03", &ret, 512, 512);
  loadTexture("../images/Star_01.png", "Star_01", &ret, 32, 32);
  loadTexture("../images/Star_02.png", "Star_02", &ret, 32, 32);
  loadTexture("../images/Star_03.png", "Star_03", &ret, 32, 32);
  loadTexture("../images/Star_04.png", "Star_04", &ret, 32, 32);
  loadTexture("../images/Star_05.png", "Star_05", &ret, 256, 256);
  loadTexture("../images/Star_04_Inside.png", "Star_04_Inside", &ret, 32, 32);
  loadTexture("../images/resource_ore.png", "resource_ore", &ret, 64, 64);
  loadTexture("../images/resource_green.png", "resource_green", &ret, 64, 64);
  loadTexture("../images/resource_purple.png", "resource_purple", &ret, 64, 64);
  loadTexture("../images/resource_orange.png", "resource_orange", &ret, 64, 64);
  loadTexture("../images/Station_Base.png", "Station_Base", &ret, 512, 512);
  loadTexture("../images/WhitePixel.png", "WhitePixel", &ret, 4, 4);
  loadTexture("../images/starField_01.png", "starField_01", &ret, 1024, 1024);
  loadTexture("../images/starField_02.png", "starField_02", &ret, 1024, 1024);
  loadTexture("../images/starField_03.png", "starField_03", &ret, 1024, 1024);
  loadTexture("../images/starField_04.png", "starField_04", &ret, 1024, 1024);
  loadTexture("../images/starField_05.png", "starField_05", &ret, 1024, 1024);
  loadTexture("../images/starField_Base.png", "starField_Base", &ret, 1024, 1024);
  loadTexture("../images/selectGlow.png", "selectGlow", &ret, 256, 256);
  loadTexture("../images/selectionBox.png", "selectionBox", &ret, 100, 100);
  loadTexture("../images/Thrust_08.png", "Thrust_08", &ret, 512, 512);
  loadTexture("../images/Thrust_Particle.png", "Thrust_Particle", &ret, 43, 42);

  return ret;
}

bool zOrderComp(gameObject *obj1, gameObject *obj2)
{
  if(obj1->getZ() < obj2->getZ())return true;
  return false;
}

//Old code
void renderObj(gameObject &obj)
{
  render *rendercomp;
  if(!(rendercomp = obj.getComponent<render>("render")))return;

  sf::VertexArray quadver(sf::Quads, 4);
  quadver[0].position = sf::Vector2f(obj.getX(), obj.getY());
  quadver[1].position = sf::Vector2f(obj.getX() + rendercomp->textXSize, obj.getY());
  quadver[2].position = sf::Vector2f(obj.getX() + rendercomp->textXSize, obj.getY() + rendercomp->textYSize);
  quadver[3].position = sf::Vector2f(obj.getX(), obj.getY() + rendercomp->textYSize);

  quadver[0].texCoords = sf::Vector2f(rendercomp->textX1, rendercomp->textY1);
  quadver[1].texCoords = sf::Vector2f(rendercomp->textX2, rendercomp->textY1);
  quadver[2].texCoords = sf::Vector2f(rendercomp->textX2, rendercomp->textY2);
  quadver[3].texCoords = sf::Vector2f(rendercomp->textX1, rendercomp->textY2);

  sf::RenderStates states;
  states.texture = rendercomp->comptexture;

  graphicsValues::pwindow->draw(quadver, states);
}

void renderObjectLists(std::vector<std::pair<sf::Texture *, sf::VertexArray>> &renderList)
{
  for(std::vector<std::pair<sf::Texture *, sf::VertexArray>>::iterator i = renderList.begin(); i != renderList.end();++i)
  {
    sf::RenderStates states;
    states.texture = i->first;

    graphicsValues::pwindow->draw(i->second, states);
  }
}

//Returns 0 if no error
int renderFrame(std::vector<gameObject *> &objects)
{
  graphicsValues::pwindow->clear();
  if(objects.empty())
  {
    graphicsValues::pwindow->display();
    return 0;
  }

  //Sort game object list by z order
  std::sort(objects.begin(), objects.end(), zOrderComp);

  float curZOrder = -10042001.0f;
  sf::Texture *curTexture = 0;
  std::vector<std::pair<sf::Texture *, sf::VertexArray>> rList;
  sf::VertexArray vertices(sf::Quads, 64);

  //Create vertex array for each texture per z order and draw it
  for(std::vector<gameObject *>::iterator i = objects.begin();i != objects.end();++i)
  {
    render *rendercomp;
    if(!(rendercomp = (*i)->getComponent<render>("render")))continue;

    if(curZOrder != (*i)->getZ())
    {
      rList.push_back(std::pair<sf::Texture *, sf::VertexArray>(curTexture, vertices));
      //Draw/clear list and update z order
      if(!rList.empty())renderObjectLists(rList);
      curZOrder = (*i)->getZ();
      rList.clear();
      vertices.clear();
    }

    if(curTexture != rendercomp->comptexture)
    {
      rList.push_back(std::pair<sf::Texture *, sf::VertexArray>(curTexture, vertices));
      vertices.clear();
      curTexture = rendercomp->comptexture;
    }

    //For rotation
    float rotCos = cos(rendercomp->rot);
    float rotSin = sin(rendercomp->rot);
    int xSiz = rendercomp->textXSize;
    int ySiz = rendercomp->textYSize;
    //Add object's vertices to vertex array
    vertices.append(sf::Vertex(sf::Vector2f((*i)->getX() + (xSiz / 2) + ((-xSiz / 2) * rotCos - (-ySiz / 2) * rotSin),
       (*i)->getY() + (ySiz / 2) + ((-xSiz / 2) * rotSin + (-ySiz / 2) * rotCos)),
      sf::Color(rendercomp->red, rendercomp->green, rendercomp->blue, rendercomp->alpha),
      sf::Vector2f(rendercomp->textX1, rendercomp->textY1)));
    vertices.append(sf::Vertex(sf::Vector2f((*i)->getX() + (xSiz / 2) + ((xSiz / 2) * rotCos - (-ySiz / 2) * rotSin),
       (*i)->getY() + (ySiz / 2) + ((xSiz / 2) * rotSin + (-ySiz / 2) * rotCos)),
      sf::Color(rendercomp->red, rendercomp->green, rendercomp->blue, rendercomp->alpha),
      sf::Vector2f(rendercomp->textX2, rendercomp->textY1)));
    vertices.append(sf::Vertex(sf::Vector2f((*i)->getX() + (xSiz / 2) + ((xSiz / 2) * rotCos - (ySiz / 2) * rotSin),
        (*i)->getY() + (ySiz / 2) + ((xSiz / 2) * rotSin + (ySiz / 2) * rotCos)),
      sf::Color(rendercomp->red, rendercomp->green, rendercomp->blue, rendercomp->alpha),
      sf::Vector2f(rendercomp->textX2, rendercomp->textY2)));
    vertices.append(sf::Vertex(sf::Vector2f((*i)->getX() + (xSiz / 2) + ((-xSiz / 2) * rotCos - (ySiz / 2) * rotSin),
        (*i)->getY() + (ySiz / 2) + ((-xSiz / 2) * rotSin + (ySiz / 2) * rotCos)),
      sf::Color(rendercomp->red, rendercomp->green, rendercomp->blue, rendercomp->alpha),
      sf::Vector2f(rendercomp->textX1, rendercomp->textY2)));

    if(rendercomp->comptexture == findTexture("Planet_01"))
    {

    }
  }
  //Render final objects
  if(vertices.getVertexCount())
  {
    rList.push_back(std::pair<sf::Texture *, sf::VertexArray>(curTexture, vertices));
    renderObjectLists(rList);
  }

  graphicsValues::pwindow->display();
  return 0;
}

//Destroys the window, as well as all other graphics components
void unloadGraphics()
{
  graphicsValues::pwindow->close();
  delete graphicsValues::pwindow;
}

//Returns a pointer to the texture with the passed name
sf::Texture *findTexture(std::string name)
{
  for(std::vector<std::tuple<std::string, sf::Texture, sf::Vector2f>>::iterator i = loadedTextures::textures.begin(); i != loadedTextures::textures.end(); ++i)
  {
    if(std::get<0>(*i) == name)return &(std::get<1>(*i));
  }
  return 0;
}

std::pair<float, float> getTextureSize(sf::Texture *text)
{
  for(std::vector<std::tuple<std::string, sf::Texture, sf::Vector2f>>::iterator i = loadedTextures::textures.begin(); i != loadedTextures::textures.end(); ++i)
  {
    if(&(std::get<1>(*i)) == text)
    {
      float x = std::get<2>(*i).x;
      float y = std::get<2>(*i).y;
      return std::pair<float, float>(x, y);
    }
  }
  return std::pair<float, float>(0.0f, 0.0f);
}
