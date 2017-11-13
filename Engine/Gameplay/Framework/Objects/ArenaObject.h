#pragma once
#include "Handle.h"
#include "VString.h"
#include "ArenaBaseObject.h"

template<typename T>
class ArenaObject : public ArenaBaseObject
{
public:
  using TemplateBase = ArenaObject<T>;
  using Base2 = ArenaBaseObject;
  using Type = T;
  ArenaObject(String tag)
    :
    tag(tag)
  {
  }
  const String tag;
};


class MeshObject : public ArenaObject<MeshObject>
{
public:
  MeshObject()
    : TemplateBase("MeshObject")
  {}
  void Update()
  {}
  void Draw(class Renderer& renderer)
  {}

  int test;
};

class GameObject : public ArenaObject<GameObject>
{
public:
  GameObject()
    : TemplateBase("GameObject")
  {}
  void Update()
  {}
  void Draw(class Renderer& renderer)
  {}

};


class Buffer
{
  Buffer(GameObject& copy)
    : copy(copy)
  {}

  GameObject copy;
};



template<typename T>
class ObjectGroup
{
  using Type = T;
public:
  void Update()
  {
    
  }

  Array<T> objects;
};