#pragma once
#include "ArenaObject.h"
#include "Rect.h"
#include <functional>
#include "Colors.h"
class UIButtonObject : public ArenaObject<UIButtonObject>
{
  enum ButtonState
  {
    NONE,
    HOVER_OVER,
    CLICKED
  };
public:
  UIButtonObject();
  void Initialize(RectF rect, std::function<void()> on_click, Color color, Color hover_color, Color pressed_color);


  void Update();
  void Draw(class Renderer& renderer);

private:
  ButtonState GetState();

  ButtonState state;
  Color state_colors[3];
  RectF rect;
  std::function<void()> on_click;
};