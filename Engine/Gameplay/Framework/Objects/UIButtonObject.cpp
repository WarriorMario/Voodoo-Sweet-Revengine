#include "UIButtonObject.h"
#include "Graphsicks\Renderer.h"
#include "Input\Input.h"

UIButtonObject::UIButtonObject()
  :
  TemplateBase("UIButtonObject")
{}

void UIButtonObject::Initialize(RectF rect, std::function<void()> on_click, Color color, Color hover_color, Color pressed_color)
{
  this->rect = rect;
  this->on_click = on_click;
  state_colors[0] = color;
  state_colors[1] = hover_color;
  state_colors[2] = pressed_color;
}

void UIButtonObject::Update()
{
  if(state == GetState())
  {
    return;
  }
  state = GetState();
  switch(state)
  {
    case NONE:
    {
      break;
    }
    case HOVER_OVER:
    {
      break;
    }
    case CLICKED:
    {
      on_click();
      break;
    }
  }
}

void UIButtonObject::Draw(Renderer & renderer)
{
  for(int i = 0; i < 2; ++i)
  {
    BackgroundShader shader;
    shader.const_data.color = state_colors[state];
    for(int y = 0; y < 3; ++y)
    {
      shader.prim_data[y] = {rect.GetTrianglePoint(i,y).x,rect.GetTrianglePoint(i,y).y, 0.0f,0.0f};
    }
    renderer.AddDrawCommand(shader);
  }
}

UIButtonObject::ButtonState UIButtonObject::GetState()
{
    return (ButtonState)((rect.Contains(input->MousePos())) ? (input->IsPressed(ButtonCode::LEFT_MOUSE) ? 2 : 1) : 0);;
}
