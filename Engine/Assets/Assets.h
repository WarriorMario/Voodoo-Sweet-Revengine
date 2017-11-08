#pragma once

#include "Singleton.h"
#include "VString.h"
#include "VTuple.h"
#include "VMap.h"
#include "VMove.h"

#include <cassert>

// ****************************************************************************
template<typename... Types>
class AssetsImpl : public Singleton<AssetsImpl<Types...>>
{
	template<typename T>
	using AssetData = typename T::Data;
	template<typename T>
	struct Asset
	{
		AssetData<T> data;
		int refs;
	};
	template<typename T>
	using AssetMap = Map<String, Asset<T>>;

public:
	template<typename T>
	void AddRef(StringRef filename)
	{
		AssetMap<T>& map = ::Get<AssetMap<T>>(data_maps);
		auto it = map.find(String(filename));
		assert(it != map.end() && "Invalid resource referenced");
		++it->second.refs;
	}
	template<typename T>
	void RemoveRef(StringRef filename)
	{
		AssetMap<T>& map = ::Get<AssetMap<T>>(data_maps);
		auto it = map.find(String(filename));
		assert(it != map.end() && "Invalid resource referenced");
		--it->second.refs;
		if (it->second.refs == 0)
		{
			// TODO: Implement free logic
		}
	}

	template<typename T>
	void AddData(StringRef filename, AssetData<T>&& value)
	{
		AssetMap<T>& map = ::Get<AssetMap<T>>(data_maps);
		assert(map.find(filename) == map.end() && "Duplicate resource loaded");
		map.at(filename) = Move(value); // use move constructor to place data in map permanently
	}
	template<typename T>
	bool HasData(StringRef filename) const
	{
		const AssetMap<T>& map = ::Get<AssetMap<T>>(data_maps);
		return map.find(String(filename)) != map.end();
	}
	template<typename T>
	const AssetData<T>& GetData(StringRef filename) const
	{
		const AssetMap<T>& map = ::Get<AssetMap<T>>(data_maps);
		auto it = map.find(String(filename));
		assert(it != map.end() && "Attempt to access invalid resource");
		return it->second.data;
	}
	template<typename T>
	const AssetData<T>& GetFallback()
	{
		const AssetMap<T>& map = ::Get<AssetMap<T>>(data_maps);
		assert(map.size() != 0 && "Attempt to access resource where none exist");
		return map.begin()->second.data;
	}

private:
	Tuple<AssetMap<Types>...> data_maps;
};

// ****************************************************************************
#include "Texture.h"
#include "Audio.h"

// This has to be defined here so resource headers can use it.
// All resource types that will be used have to be added here.
using Assets = AssetsImpl<Audio, Texture>;
