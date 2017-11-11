#include "DirectoryWatch.h"

// ****************************************************************************
bool DirectoryWatch::Make(DirectoryWatch& result, StringRef fullpath, bool include_subdirectories)
{
  char dirpath[MAX_PATH];
  GetFullPathNameA(fullpath.data(), CountOf(dirpath), dirpath, nullptr);

  HANDLE tmp_handles[3];

  tmp_handles[0] = FindFirstChangeNotificationA(
    dirpath,
    (BOOL)include_subdirectories,
    FILE_NOTIFY_CHANGE_DIR_NAME);
  if(tmp_handles[0] == INVALID_HANDLE_VALUE)
  {
    Logger::Get().Write(LogCategory::ENGINE, "DirectoryWatch error with code: %x", GetLastError());
    return false;
  }

  tmp_handles[1] = FindFirstChangeNotificationA(
    dirpath,
    (BOOL)include_subdirectories,
    FILE_NOTIFY_CHANGE_FILE_NAME);
  if(tmp_handles[1] == INVALID_HANDLE_VALUE)
  {
    Logger::Get().Write(LogCategory::ENGINE, "DirectoryWatch error with code: %x", GetLastError());
    return false;
  }

  tmp_handles[2] = FindFirstChangeNotificationA(
    dirpath,
    (BOOL)include_subdirectories,
    FILE_NOTIFY_CHANGE_LAST_WRITE);
  if(tmp_handles[3] == INVALID_HANDLE_VALUE)
  {
    Logger::Get().Write(LogCategory::ENGINE, "DirectoryWatch error with code: %x", GetLastError());
    return false;
  }

  // assign to result
  for(int i = 0; i < 3; ++i)
  {
    result.handles[i] = tmp_handles[i];
  }

  return true;
}