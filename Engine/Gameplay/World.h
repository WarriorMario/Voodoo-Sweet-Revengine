#pragma once

class Renderer;
class World
{
public:
	World();
	void Init();
	void Tick(float delta_time);
	void Draw(Renderer& renderer);

private:
	// data stuffs
};