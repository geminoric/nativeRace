#include <vector>
#include "game_object.hpp"

bool gameObject::hasComponent(int componentID)
{
  for(std::vector<std::shared_ptr<component>>::iterator i = components.begin(); i != components.end(); ++i)
  {
    if(componentID == (*i)->id)return true;
  }
  return false;
}

void gameObject::addComponent(component *newComponent)
{
  newComponent->owner = this;
  components.push_back(std::shared_ptr<component>(newComponent));
}
