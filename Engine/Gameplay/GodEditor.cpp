#include "GodEditor.h"
#include "Framework\Arena.h"
#include "Graphsicks\Graphics.h"

void GodEditor::Init()
{
  // Create UI element per object type
  InitGroupUI(placeable_objects, Vec2( 50, Graphics::ScreenHeight - 50));
}

void GodEditor::Update()
{
  MoveSelectedObject(placeable_objects, selected_index);
}
