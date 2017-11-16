#pragma once
#include "VArray.h"
#include "Framework\Arena.h"
#include "Input\Input.h"
class ArenaBaseObject;

class GodEditor
{
public:
  GodEditor(Arena& arena):
    arena(arena)
  {}

  void Init();
  void Update();
  template<typename T>
  void PlaceObject()
  {
    DestroyAllObject(placeable_objects, selected_index);
    Get<Handle<T>>(placeable_objects) = arena.Create<T>();
    selected_index = Index<T, Tuple<OBJECT_TYPES>>::value;
  }
private:
  Arena& arena;
  template<typename... Object>
  using ObjectGroup = Tuple<Handle<Object>...>;

  template <class T, class Tuple>
  struct Index;

  template <class T, class... Types>
  struct Index<T, Tuple<T, Types...>>
  {
    static const size_t value = 0;
  };

  template <class T, class U, class... Types>
  struct Index<T, Tuple<U, Types...>>
  {
    static const size_t value = 1 + Index<T, Tuple<Types...>>::value;
  };

  template<size_t I = 0, typename... Types>
  typename std::enable_if_t<I == sizeof...(Types), void>
    InitGroupUI(Tuple<Types...>&,Vec2 pos)
  {}
  template<size_t I = 0, typename... Types>
  typename std::enable_if_t < I<sizeof...(Types), void>
    InitGroupUI(Tuple<Types...>& tuple, Vec2 pos)
  {
    std::function<void()> select_object = [=]()
    {
      this->PlaceObject<std::tuple_element<I, Tuple<OBJECT_TYPES>>::type>();
    };
    
    auto handle = arena.Create<UIButtonObject>();
    handle->Initialize(RectF(0 + pos.y, 50 + pos.y, I * 50 + pos.x, I * 50 + 50 + pos.x), select_object, Colors::White, Colors::Yellow, Colors::Green);
    Get<I>(placeable_objects) = arena.Create<std::tuple_element<I, Tuple<OBJECT_TYPES>>::type>();

    InitGroupUI<I + 1, Types...>(tuple, pos);
  }

  // Temp fix Maarton help
  template<size_t I = 0, typename... Types>
  typename std::enable_if_t<I == sizeof...(Types), void>
    DestroyAllObject(Tuple<Types...>&, size_t index)
  {}
  template<size_t I = 0, typename... Types>
  typename std::enable_if_t < I<sizeof...(Types), void>
    DestroyAllObject(Tuple<Types...>& tuple, size_t index)
  {
    if(I == index)
    {
      arena.Destroy(Get<I>(placeable_objects));
      return;
    }
    DestroyAllObject<I + 1, Types...>(tuple,index);
  }

  template<size_t I = 0, typename... Types>
  typename std::enable_if_t<I == sizeof...(Types), void>
    MoveSelectedObject(Tuple<Types...>&, size_t index)
  {}
  template<size_t I = 0, typename... Types>
  typename std::enable_if_t < I<sizeof...(Types), void>
    MoveSelectedObject(Tuple<Types...>& tuple, size_t index)
  {
    if(I == index)
    {
      Get<I>(placeable_objects)->SetPosition(ArenaBaseObject::input->MousePos());
      return;
    }
    MoveSelectedObject<I + 1, Types...>(tuple, index);
  }


  ObjectGroup<OBJECT_TYPES> placeable_objects;
  Array<Handle<UIButtonObject>> ui_objects;
  size_t selected_index;
};

