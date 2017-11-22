#include "Arena.h"

Arena::Arena(Input & input)
  :
  physx()
{
  ArenaBaseObject::arena = this;
  ArenaBaseObject::input = &input;
}