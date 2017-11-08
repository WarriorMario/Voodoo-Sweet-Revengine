#pragma once

#include "Resource.h"
#include "Colors.h"

// ****************************************************************************
class Texture : public Resource<Texture>
{
public:
	struct Data
	{
		Data(Data&& source)
			:
			width(source.width),
			height(source.height),
			texels(source.texels)
		{
			source.texels = nullptr;
		}
		~Data()
		{
			if (texels)
			{
				delete texels;
			}
		}

		int width, height;
		Color* texels;
	};

public:
	Texture(StringRef filename)
		: Base(filename)
	{
	}

private:
	friend bool Load(StringRef filename, Data& data)
	{
		// TODO: Implement load logic

		return false;
	}
};