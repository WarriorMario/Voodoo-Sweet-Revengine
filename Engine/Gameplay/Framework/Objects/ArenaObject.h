#pragma once
#include "Handle.h"
#include "VString.h"
#include "ArenaBaseObject.h"
#include "Utility.h"
#include "Utility\Logging.h"

#include <typeinfo>

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

  void SetPosition(struct Vec2& pos)
  {
    Logger::Get().Write(LogCategory::USER, "Missing implementation of %s::SetPosition()", typeid(T).name());
    CRASH();
  }
  const String tag;
  static T* dummy_instance;
};

template<typename T>
T* ArenaObject<T>::dummy_instance =(T*) malloc(sizeof( T));

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
