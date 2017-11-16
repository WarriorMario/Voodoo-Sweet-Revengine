#include "Arena.h"
#include "Gameplay\GodEditor.h"
//#include "Arena.h"
//
//void Arena::Update()
//{
//  UpdateGroup(objectGroups);
//}

Arena::Arena(Input & input)
  :
  physx()
{
  ArenaBaseObject::arena = this;
  ArenaBaseObject::input = &input;
}