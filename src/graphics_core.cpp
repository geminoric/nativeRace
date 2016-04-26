#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include "graphics_core.hpp"
#include "graphics_components.hpp"
#include "game.hpp"
#include "game_object.hpp"

const int MAX_FRAMERATE = 60;

namespace
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
  pwindow = new sf::RenderWindow(sf::VideoMode(resX, resY), windowTitle);

  sf::View view(sf::FloatRect(0, 0, 1920 * 8, 1080 * 8));
  pwindow->setView(view);

  pwindow->setFramerateLimit(MAX_FRAMERATE);
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
  loadTexture("../images/testship.png", "testShip", &ret, 512, 512);

  return ret;
}

bool zOrderComp(gameObject *obj1, gameObject *obj2)
{
  if(obj1->getZ() < obj2->getZ())return true;
  return false;
}

void renderObj(gameObject &obj)
{
  render *rendercomp;
  if(!(rendercomp = obj.getComponent<render>("render")))return;
  
  /*
  std::pair<float, float> textSize = getTextureSize(rendercomp->comptexture);
  float textX = textSize.first;
  float textY = textSize.second;
  */

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
  //sf::Transform trans;
  //trans.scale(rendercomp->scaleX, rendercomp->scaleY, obj.getX() + rendercomp->sizeX / 2, obj.getY() + rendercomp->sizeY / 2);
  //states.transform = trans;
  states.texture = rendercomp->comptexture;

  pwindow->draw(quadver, states);
}

void renderObjectLists(std::vector<std::pair<sf::Texture *, sf::VertexArray>> &renderList)
{
  for(std::vector<std::pair<sf::Texture *, sf::VertexArray>>::iterator i = renderList.begin(); i != renderList.end();++i)
  {
    sf::RenderStates states;
    states.texture = i->first;

    pwindow->draw(i->second, states);
  }
}

//Returns 0 if no error
int renderFrame(std::vector<gameObject *> &objects)
{
  pwindow->clear();
  if(objects.empty())
  {
    pwindow->display();
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

    //Add object's vertices to vertex array
    vertices.append(sf::Vertex(sf::Vector2f((*i)->getX(), (*i)->getY()), sf::Vector2f(rendercomp->textX1, rendercomp->textY1)));
    vertices.append(sf::Vertex(sf::Vector2f((*i)->getX() + rendercomp->textXSize, (*i)->getY()), sf::Vector2f(rendercomp->textX2, rendercomp->textY1)));
    vertices.append(sf::Vertex(sf::Vector2f((*i)->getX() + rendercomp->textXSize, (*i)->getY() + rendercomp->textYSize), sf::Vector2f(rendercomp->textX2, rendercomp->textY2)));
    vertices.append(sf::Vertex(sf::Vector2f((*i)->getX(), (*i)->getY() + rendercomp->textYSize), sf::Vector2f(rendercomp->textX1, rendercomp->textY2)));
  }
  //Render final objects
  if(vertices.getVertexCount())
  {
    rList.push_back(std::pair<sf::Texture *, sf::VertexArray>(curTexture, vertices));
    renderObjectLists(rList);
  }

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
        statusValues::gameIsRunning = 0;
  }
}

//Destroys the window, as well as all other graphics components
void unloadGraphics()
{
  pwindow->close();
  delete pwindow;
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
