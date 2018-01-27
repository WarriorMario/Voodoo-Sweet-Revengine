#include "SpriteAnimation.h"
#include "Utility\DrawUtils.h"
#include "VString.h"

SpriteAnimation::SpriteAnimation(StringRef clipname, int frame_count, float delay) :
  frame_count(frame_count),
  delay(delay),
  curr_frame(0),
  elapsed(0.0f)
{
  for(int i = 0; i < frame_count; ++i)
  {
    String filename;
    filename += clipname;
    filename += '_';
    filename += std::to_string(i);
    filename += ".png"; // Hackity-hacked extension
    frames.emplace_back(new Texture(filename));
  }
}

void SpriteAnimation::Update(float dt)
{
  elapsed += dt;
  if(elapsed >= delay)
  {
    ++curr_frame;
    curr_frame = curr_frame == frame_count ? 0 : curr_frame;
    elapsed = 0.0f;
  }
}
void SpriteAnimation::Draw(Vec2 pos, Vec2 size, bool flip_x, bool flip_y)
{
  RenderSharedQuad<ForegroundShader>(
    pos, size,
    flip_x, flip_y,
    Colors::White, frames[curr_frame]);
}
