#include "GodEditor.h"
#include "Framework\Arena.h"
#include "Graphsicks\Graphics.h"

void GodEditor::Init()
{
  // Create UI element per object type
  ForEach(placeable_objects, UIInitializer(), arena, *this, Vec2(50, Graphics::ScreenHeight - 50));
}

void GodEditor::Update()
{
  if(ArenaBaseObject::input->IsPressed(ButtonCode::LEFT_MOUSE))
  {
    OnElement(placeable_objects, selected_index, ObjectPlacer(), *this);
    //OutputDebugStringA("Why Maarton");
  }
  OnElement(placeable_objects, selected_index, ObjectMover());
}
