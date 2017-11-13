#pragma once

#include "VString.h"
#include "Utility/Logging.h"
#include "Utility.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

// ****************************************************************************
enum class DirectoryChange
{
  NOTHING = 0,
  DIR,
  FILE,
  WRITE
};

// ****************************************************************************
class DirectoryWatch
{
public:
  DirectoryWatch()
  {
    for(auto& handle : handles)
    {
      handle = INVALID_HANDLE_VALUE;
    }
  }
  ~DirectoryWatch()
  {
    for(auto& handle : handles)
    {
      FindCloseChangeNotification(handle);
    }
  }

  static bool Make(DirectoryWatch& result, StringRef fullpath, bool include_subdirectories = false);

  DirectoryChange PeekChanges()
  {
    assert(!RangeEquals(handles, INVALID_HANDLE_VALUE) && "Uninitialized directory watch used");

    DWORD status = WaitForMultipleObjects((DWORD)CountOf(handles), handles, FALSE, 0);
    switch(status)
    {
      case WAIT_OBJECT_0: // directory
        FindNextChangeNotification(handles[0]);
        return DirectoryChange::DIR;
      case WAIT_OBJECT_0 + 1: // file
        FindNextChangeNotification(handles[1]);
        return DirectoryChange::FILE;
      case WAIT_OBJECT_0 + 2: // write
        FindNextChangeNotification(handles[2]);
        return DirectoryChange::WRITE;
    }
    return DirectoryChange::NOTHING;
  }

private:
  HANDLE handles[3]; // watch directory, file, and write changes

};