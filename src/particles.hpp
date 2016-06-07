#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include "game_object.hpp"
#include "graphics_components.hpp"

class particle : public component
{
public:
  gameObject *owner;
  render *pRender;
  int velX, velY;
  float decelMult;
  int fadePerFrame;

  particle(gameObject *parent, int vX, int vY, float decelMultiplier, int fadePerFrame_, const char *tName, int textX, int textY, int siz);

  void onUpdate();
  const char *getCompName() { return "particle"; }
};

#endif
