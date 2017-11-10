#pragma once
#include "ArenaObject.h"
#include "Physics\Physics.h"
#include "VTuple.h"
#include "VArray.h"
template<typename T>
struct RefCounter : public T
{
  RefCounter(const T& obj)
    : T(obj)
  {

  }
  void IncrementCount()
  {
    count++;
  }
  void DecrementCount()
  {
    assert(count == 0);
    count--;
  }
private:
  size_t count;
};

class Arena
{
public:

  Arena()
    :
    physx()
    //input(input)
  {

    InitGroups(objectGroups);
    Handle<MeshObject> s = Create<MeshObject>();
    s->test = 1;
  }

  template<typename T>
  Handle<T> Create()
  {
    return Register(T());
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
  }
private:
  Physics physx;
  // class Input& input;


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
    InitGroups(Tuple<Array<RefCounter<Types>>...>&, Args&&...)
  {}
  template<size_t I = 0, typename... Types, typename... Args>
  typename std::enable_if_t < I<sizeof...(Types), void>
    InitGroups(Tuple<Array<RefCounter<Types>>...>& tuple, Args&&... args)
  {
    Handle<Types>::arena = this;
    InitGroups<I + 1, Types...>(tuple, args...);
  }
public:
  ObjectGroup<GameObject, MeshObject> objectGroups;
};
