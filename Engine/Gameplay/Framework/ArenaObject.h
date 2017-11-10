#pragma once
#include "Handle.h"
#include "VString.h"

class Arena;

template<typename T>
class ArenaObject
{
public:
  using Base = ArenaObject<T>;
  using HandleType = Handle<T>;
  ArenaObject(String tag)
    :
    tag(tag)
  {
  }
  const String tag;

private:
  int refs;
  static Arena* arena;
};


template<typename T>
Arena* ArenaObject<T>::arena = nullptr;

class MeshObject : public ArenaObject<MeshObject>
{
public:
  MeshObject()
    : Base("MeshObject")
  {}
  void Update()
  {}

  int test;
};

class GameObject : public ArenaObject<GameObject>
{
public:
  GameObject()
    : Base("GameObject")
  {}
  void Update()
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
public:
  void Update()
  {
  }

  Array<T> objects;
};