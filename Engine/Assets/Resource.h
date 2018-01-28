#pragma once

#include "VString.h"
#include "Utility.h"

// ****************************************************************************
// To create a new resource you can derive it from Resource, using itself as
// value for parameter T. The resource must implement:
// - A public member struct Data, which implements:
//  * A move constructor
//  * A destructor (which tests for validity of the resource before releasing)
// - A public constructor which passes a filename to the base class
// - A private friend function bool Load(StringRef filename, Data& data), which
//   returns:
//   * true = succesfull loading. Places valid data in the passed destination.
//   * false = load failure.

// ****************************************************************************
template<typename T>
class Resource
{
public:
  using Base = Resource<T>;

  Resource() :
    data(&Assets::Get().GetFallback<T>()),
    is_fallback(true)
  {
  }
  Resource(const Resource& other)
  {
    *this = other;
  }
  ~Resource()
  {
    if(!is_fallback)
    {
      Assets::Get().RemoveRef<T>(filename);
    }
  }

  Resource& operator=(const Resource& other)
  {
    filename = other.filename;
    data = other.data;
    is_fallback = other.is_fallback;
    if(!is_fallback)
    {
      Assets::Get().AddRef<T>(filename);
    }

    return *this;
  }

  void Reload()
  {
    T::Data* tmp_data = (T::Data*)::operator new(sizeof(T::Data));
    ZeroMem(tmp_data, sizeof(T::Data));
    if(!Load(filename, *tmp_data))
    {
      // if load failed, keep existing resource
      delete tmp_data;
    }
    else
    {
      if(is_fallback)
      {
        data = &Assets::Get().AddData<T>(filename, Move(*tmp_data));
      }
      else
      {
        Assets::Get().ReplaceData<T>(filename, Move(*tmp_data));
      }
    }
  }

protected:
  Resource(StringRef filename)
    :
    data(nullptr),
    filename(filename),
    is_fallback(false)
  {
    if(Assets::Get().HasData<T>(filename))
    {
      data = &Assets::Get().GetData<T>(filename);
    }
    else
    {
      T::Data* tmp_data = (T::Data*)::operator new(sizeof(T::Data));
      ZeroMem(tmp_data, sizeof(T::Data));// This only works for POD types
      if(!Load(filename, *tmp_data))
      {
        delete tmp_data;

        // if load failed, use fallback instead
        data = &Assets::Get().GetFallback<T>();
        is_fallback = true;
      }
      else
      {
        data = &Assets::Get().AddData<T>(filename, Move(*tmp_data));
      }
    }
  }

  template<typename Resource>
  const typename Resource::Data& GetData() const
  {
    return *((const typename Resource::Data*)data);
  }

private:
  String filename;
  const void* data;
  bool is_fallback;

};