//NOTE :  TRY TO PUT OBJECTS WITH THE SAME TEXTURE ON THE SAME UNIQUE Z ORDER
//        TRY ALSO TO PUT OBJECTS ON UNIQUE Z ORDER

#ifndef GRAPHICS_CORE_HPP
#define GRAPHICS_CORE_HPP

#include <string>
#include <vector>

class gameObject;

namespace sf
{
  class Texture;
}

//Returns 0 if no error
int initGraphics(int resX, int resY, const char *windowTitle);
int loadAllTextures();
int renderFrame(std::vector<gameObject *> &objects);
//Destroys the window, as well as all other graphics components
void unloadGraphics();
void loadTexture(const char *loc, const char *name, int *currentret);
void loadTextureFromSpriteSheet(const char *loc, const char *name, int *currentret,
                                int xSize, int ySize, int xLeftSpriteSheet, int yTopSpriteSheet);
sf::Texture *findTexture(std::string name);
std::pair<float, float> getTextureSize(sf::Texture *text);

#endif
