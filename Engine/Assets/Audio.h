#pragma once

#include "Resource.h"

// ****************************************************************************
class Audio : public Resource<Audio>
{
public:
	struct Data
	{
		Data(Data&& source)
			:
			waves(source.waves),
			length(source.length)
		{
			source.waves = nullptr;
		}
		~Data()
		{
			if (waves)
			{
				delete waves;
			}
		}

		void* waves;
		size_t length;
	};

public:
	Audio(StringRef filename)
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