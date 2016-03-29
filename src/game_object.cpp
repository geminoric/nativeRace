#include "game_object.hpp"

bool gameObject::hasComponent(int componentID)
{
  for(std::vector<std::shared_ptr<component>>::iterator i = components.begin(); i != components.end(); ++i)
  {
    if(componentID == (*i)->id)return true;
  }
  return false;
}