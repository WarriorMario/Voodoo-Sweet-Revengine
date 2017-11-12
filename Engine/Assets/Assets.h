#pragma once

#include "Singleton.h"
#include "VString.h"
#include "VTuple.h"
#include "VMap.h"
#include "VTraits.h"
#include "Utility/Logging.h"
#include "Utility.h"

#include <typeinfo>

// ****************************************************************************
template<typename... Types>
class AssetsImpl : public Singleton<AssetsImpl<Types...>>
{
  template<typename T>
  using AssetData = typename T::Data;
  template<typename T>
  struct Asset
  {
    using AssetType = T;
    AssetData<T> data;
    int refs;
  };
  template<typename T>
  using AssetMap = UnorderedMap<String, Asset<T>>;

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
    if(it->second.refs == 0)
    {
      map.erase(it);
    }
  }

  template<typename T>
  const AssetData<T>& AddData(StringRef filename, AssetData<T>&& value)
  {
    Asset<T> new_asset = {Move(value), 1};

    AssetMap<T>& map = ::Get<AssetMap<T>>(data_maps);
    assert(map.find(String(filename)) == map.end() && "Duplicate resource loaded");
    auto it = map.emplace((String)filename, Move(new_asset)).first; // use move constructor to place data in map permanently
    return it->second.data;
  }
  template<typename T>
  void ReplaceData(StringRef filename, AssetData<T>&& value)
  {
    AssetMap<T>& map = ::Get<AssetMap<T>>(data_maps);
    auto it = map.find(String(filename));
    assert(it != map.end() && "Attempt to replace resource where none exists");
    it->second.data.~AssetData<T>(); // deconstruct existing object
    new (&it->second.data) AssetData<T>(Move(value)); // use move constructor to place data in map permanently
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
  struct MapClearer
  {
    template<typename T>
    void operator()(T& map)
    {
      if(map.size() > 0)
      {
        const size_t amount = map.size();
        const char* name = typeid(T::mapped_type::AssetType).name();
        Logger::Get().Write(LogCategory::ENGINE, "Asset leak detected! %d instance(s) of %s left", amount, name);
      }
      map.clear();
    }
  };

public:
  void CleanUp()
  {
    ForEach(data_maps, MapClearer());
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
