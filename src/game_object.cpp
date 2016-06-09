#include <vector>
#include <iostream>
#include "game_object.hpp"

void gameObject::addComponent(component *newComponent)
{
  newComponent->owner = this;
  components.push_back(std::shared_ptr<component>(newComponent));
}

gameObject::~gameObject()
{
}
