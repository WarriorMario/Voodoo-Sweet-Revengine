#include "Shader.h"
template<>
Color ShadeOperation<BackgroundShader>::operator()(PixelData& pixel_data, ConstData& const_data)
{
	return Colors::White;
}