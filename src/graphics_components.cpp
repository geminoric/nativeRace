#include "graphics_components.hpp"

bool render::idAlreadyCreated = false;

render::render()
{
  //Increment the id 
  if(idAlreadyCreated)return;
  id = totalID++;
  idAlreadyCreated = true;
}


