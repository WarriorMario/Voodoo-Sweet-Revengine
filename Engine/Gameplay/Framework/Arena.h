#pragma once
#include "Objects\PhysicsObject.h"
#include "Objects\ParticleObject.h"
#include "Objects\UIButtonObject.h"
#include "Physics\Physics.h"
#include "VTuple.h"
#include "VArray.h"
#define OBJECT_TYPES GameObject, MeshObject, PhysicsObject, ParticleObject, UIButtonObject
template<typename T>
struct RefCounter : public T
{
  using Type = typename T;
  RefCounter(const T& obj)
    : T(obj)
  {
    count = 1;
  }
  void IncrementCount()
  {
    assert(IsValid());
    count++;
  }
  void DecrementCount()
  {
    assert(count == 0&& IsValid());
    count--;
  }
  bool IsValid()
  {
    return (count != INVALID_COUNTER);
  }
  void Invalidate()
  {
    count = INVALID_COUNTER;
  }
private:
  static const size_t INVALID_COUNTER = -1;
  size_t count;
};

class Arena
{
public:

  Arena(Input& input);

  template<typename T>
  Handle<T> Create()
  {
    return Register(T());
  }
  template<typename T>
  void Destroy(Handle<T>& handle)// We should design this more 
  {
    if(handle.IsValid())
    {
      RefCounter<T>& ref =  Get<Array<RefCounter<T>>>(objectGroups)[GetIndex(handle)];
      ref.Invalidate();
      handle.Invalidate();
    }
  }

  template<typename T>
  Handle<T> Register(T& object)
  {
    static_assert(std::is_base_of_v<ArenaObject<T>, T>, "T is not an ArenaObject");
    auto as = Get<Array<RefCounter<T>>>(objectGroups);
    
    Handle<T> ret = Get<Array<RefCounter<T>>>(objectGroups).size();
    RefCounter<T> refObj = object;
    Get<Array<RefCounter<T>>>(objectGroups).push_back(refObj);
    return ret;
  }
  template<typename T>
  void AddRef(Handle<T> handle)
  {
    static_assert(std::is_base_of_v<ArenaObject<T>, T>, "T is not an ArenaObject");
    if(handle.IsValid())
    {
      Get<Array<RefCounter<T>>>(objectGroups)[GetIndex(handle)].IncrementCount();
    }
  }
  template<typename T>
  void RemoveRef(Handle<T> handle)
  {
    if(handle.IsValid())
    {
      Get<Array<RefCounter<T>>>(objectGroups)[GetIndex(handle)].DecrementCount();
    }
  }
  void Update()
  {
    UpdateGroup(objectGroups);
    physx.Update();
  }

  void Draw(class Renderer& renderer)
  {
    DrawGroup(objectGroups,renderer);
  }
  Physics physx;
private:


  template<typename... Object>
  using ObjectGroup = Tuple<Array<RefCounter<Object>>...>;

  template<size_t I = 0, typename... Types, typename... Args>
  typename std::enable_if_t<I == sizeof...(Types), void>
    UpdateGroup(Tuple<Types...>&, Args&&...)
  {}
  template<size_t I = 0, typename... Types, typename... Args>
  typename std::enable_if_t < I<sizeof...(Types), void>
    UpdateGroup(Tuple<Types...>& tuple, Args&&... args)
  {
    auto& group = Get<I>(tuple);
    for(int i = 0; i < group.size(); ++i)
    {
      group[i].Update();
    }
    UpdateGroup<I + 1, Types...>(tuple, args...);
  }

  template<size_t I = 0, typename... Types, typename... Args>
  typename std::enable_if_t<I == sizeof...(Types), void>
    DrawGroup(Tuple<Types...>&, Args&&...)
  {}
  template<size_t I = 0, typename... Types, typename... Args>
  typename std::enable_if_t < I<sizeof...(Types), void>
    DrawGroup(Tuple<Types...>& tuple, Args&&... args)
  {
    auto& group = Get<I>(tuple);
    for(int i = 0; i < group.size(); ++i)
    {
      group[i].Draw(args...);
    }
    DrawGroup<I + 1, Types...>(tuple, args...);
  }


public:
  ObjectGroup<OBJECT_TYPES> objectGroups;
};
