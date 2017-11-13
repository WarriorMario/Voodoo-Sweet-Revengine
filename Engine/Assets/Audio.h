#pragma once
#include "Sound.h"
#include "Resource.h"
// ****************************************************************************
class Audio : public Resource<Audio>
{
public:
  struct Data
  {
    Data(Data&& source)
      :
      sound(Move(source.sound))
    {
    }

    ~Data()
    {
    }
    Sound sound;
  };

public:
  Audio(StringRef filename)
    : Base(filename)
  {}

  void Play(float frequency_mod = 1.0f, float volume = 1.0f)
  {
    Data* data = (Data*)const_cast<void*>( reinterpret_cast<const void*>(GetData<Audio>()));
    data->sound.Play(frequency_mod, volume);
  }

private:
  friend bool Load(StringRef filename, Data& data)
  {
    if(filename.empty())
    {
      return false;
    }
    // Convert to wide string as chili uses that
    wchar_t* buf = new wchar_t[filename.size() * 2 + 2 + 9];
    swprintf(buf, L"Sound/%S", filename.data()); 
    std::wstring wide_filename = buf;
    (new(&data.sound) Sound());
    data.sound = Sound(wide_filename);
    delete[] buf;
    return true;
  }
};