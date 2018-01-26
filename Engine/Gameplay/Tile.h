#pragma once



class Texture;
class Renderer;
class Tile
{
public:
	static constexpr unsigned int size = 32;

	Tile();
	Tile(Texture* tile_texture);
	void Init();
	void Render(Renderer& renderer);
private:
	Texture* tex;
};