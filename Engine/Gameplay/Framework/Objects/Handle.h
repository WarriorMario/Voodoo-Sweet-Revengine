#pragma once
#include "VArray.h"
class Arena;

template<typename T>
class Handle
{
public:
  using Type = T;
  Handle() :index(INVALID_HANDLE)
  {}
  Handle(size_t index) :index(index)
  {}

  bool IsValid() const
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

  T* operator->() const
  {
    //assert(IsValid());
    
    auto test = Get<Array<RefCounter<T>>>(T::arena->objectGroups)[index];
    if(test.IsValid())
    {
      return &(Get<Array<RefCounter<T>>>(T::arena->objectGroups)[index]);
    }
    else
    {
      return T::dummy_instance;
    }
  }
private:
  size_t index;
  inline friend size_t GetIndex(Handle& handle)
  {
    return handle.index;
  }
};