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
  {
    selected_index = -1;
  }

  void Init();
  void Update();
  template<typename T>
  void SelectObject()
  {
    OnElement(placeable_objects, selected_index, ObjectDestroyer(), arena);
    Get<Handle<T>>(placeable_objects) = arena.Create<T>();
    selected_index = IndexValue<T, Tuple<OBJECT_TYPES>>;
  }
  template<typename T>
  void PlaceObject()
  {
    Get<Handle<T>>(placeable_objects) = arena.Create<T>();
  }
private:
  Arena& arena;
  template<typename... Object>
  using ObjectGroup = Tuple<Handle<Object>...>;

  struct UIInitializer
  {
    template<typename T>
    void operator()(T& type, Arena& arena, GodEditor& editor, Vec2 pos)
    {
      std::function<void()> select_object = [&]()
      {
        editor.SelectObject<T::Type>();
      };
      auto handle = arena.Create<UIButtonObject>();
      const int I = IndexValue<T, ObjectGroup<OBJECT_TYPES>>;
      handle->Initialize(RectF(0 + pos.y, 50 + pos.y, I * 50 + pos.x, I * 50 + 50 + pos.x), select_object, Colors::White, Colors::Yellow, Colors::Green);
      type = arena.Create<T::Type>();
    }
  };
  
  struct ObjectDestroyer
  {
    template<typename T>
    void operator()(T& object, Arena& arena)
    {
      arena.Destroy(object);
    }
  };
  struct ObjectMover
  {
    template<typename T>
    void operator()(T& object)
    {
      object->SetPosition(ArenaBaseObject::input->MousePos());
    }
  };
  struct ObjectPlacer
  {
    template <typename T>
    void operator()(T& object, GodEditor& editor)
    {
      editor.PlaceObject<T::Type>();
    }
  };


  ObjectGroup<OBJECT_TYPES> placeable_objects;
  Array<Handle<UIButtonObject>> ui_objects;
  size_t selected_index;
};

