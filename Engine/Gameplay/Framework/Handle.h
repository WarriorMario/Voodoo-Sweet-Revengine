#pragma once
#include "VArray.h"

template<typename Type>
class Handle
{
public:
  Handle() :index(INVALID_HANDLE)
  {}
  Handle(size_t index) :index(index)
  {}

  bool IsValid()
  {
    return index == INVALID_HANDLE;
  }
  void Invalidate()
  {
    index = INVALID_HANDLE;
  }
  operator bool()
  {
    return IsValid();
  }
  bool operator!()
  {
    return !IsValid();
  }
  static constexpr size_t INVALID_HANDLE = -1;

  Type* operator->() const
  {
    return &(Get<Array<RefCounter<Type>>>(arena->objectGroups)[index]);
  }
private:
  size_t index;
  static class Arena* arena;
  static friend void SetArena(Arena* arena_ptr)
  {
    arena = arena_ptr;
  }

  inline friend size_t GetIndex(Handle& handle)
  {
    return handle.index;
  }
};