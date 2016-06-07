#include <iostream>
#include "particles.hpp"
#include "game.hpp"

#define MIN_ALPHA 25

particle::particle(gameObject *parent, int vX, int vY, float decelMultiplier, int fadePerFrame_, const char *tName, int textX, int textY, int siz) : 
owner(parent), velX(vX), velY(vY), decelMult(decelMultiplier), fadePerFrame(fadePerFrame_)
{
  owner->addComponent(new render(tName, 0, textX, 0, textY, siz, siz));
  pRender = owner->getComponent<render>("render");
}

void particle::onUpdate()
{
  pRender->alpha -= fadePerFrame;

  owner->x += velX;
  owner->y += velY;
  velX *= decelMult;
  velY *= decelMult;
  if(pRender->alpha < MIN_ALPHA)gameControl::deleteObject(owner);
}
