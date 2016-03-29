#include "graphics_components.hpp"

bool render::idAlreadyCreated = false;

render::render()
{
  //Increment the id 
  if(idAlreadyCreated)return;
  id = totalID++;
  idAlreadyCreated = true;
}

render::render(sf::Texture *texture) : comptexture(texture)
{
  //Increment the id 
  if(idAlreadyCreated)return;
  id = totalID++;
  idAlreadyCreated = true;
}

render::render(const char *textName) : comptexture(findTexture(textName))
{
  //Increment the id 
  if(idAlreadyCreated)return;
  id = totalID++;
  idAlreadyCreated = true;
}

render::~render()
{
  
}


