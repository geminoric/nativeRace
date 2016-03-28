#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include <iostream>
#include "graphics_core.hpp"

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
  std::vector<std::pair<std::string, sf::Texture>> textures;
}

//Returns 0 if no error
int initGraphics(int resX, int resY, const char *windowTitle)
{
  pwindow = new sf::RenderWindow(sf::VideoMode(resX, resY), windowTitle);
  return 0;
}

void loadTexture(const char *loc, const char *name, int *currentret)
{
  sf::Texture ntext;
  if(!ntext.loadFromFile(loc))
  {
    std::cout << "[ERROR] Could not load " << loc << std::endl;
    if(currentret != 0)++(*currentret);
  }
  else
  {
    loadedTextures::textures.push_back(std::pair<std::string, sf::Texture>(name, ntext));
  }
}

int loadAllTextures()
{
  int ret = 0;
  
  loadTexture("../images/testship.png", "testship.png", &ret);

  return ret;
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
  for(std::vector<std::pair<std::string, sf::Texture>>::iterator i = loadedTextures::textures.begin(); i != loadedTextures::textures.end(); ++i)
  {
    if(i->first == name)return &(i->second);
  }
  return 0;
}
