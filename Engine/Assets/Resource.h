#pragma once

#include "VString.h"

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

  Resource(const Resource& other)
  {
    *this = other;
  }
  Resource(Resource&& other)
  {
    *this = std::move(other);
  }
  ~Resource()
  {
    if(data)
    {
      Assets::Get().RemoveRef<T>(filename);
    }
  }

  Resource& operator=(const Resource& other)
  {
    data = other.data;
  }
  Resource& operator=(Resource&& other)
  {
    data = other.data;
    other.data = nullptr
  }

protected:
  Resource(StringRef filename)
    :
    data(nullptr),
    filename(filename)
  {
    if(Assets::Get().HasData<T>(filename))
    {
      data = &Assets::Get().GetData<T>(filename);
    }
    else
    {
      data = ::operator new(sizeof(T::Data));
      if(!Load(filename, *(T::Data*)data))
      {
        delete data;

        // if load failed, use fallback instead
        data = &Assets::Get().GetFallback<T>();
      }
    }
  }

private:
  String filename;
  const void* data;

};