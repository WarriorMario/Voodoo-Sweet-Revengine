#pragma once

#include <fstream>
#include <type_traits>
#include "VString.h"

// ****************************************************************************
#define LOG_MAP(x) \
	x(GAME  , 1) \
	x(ENGINE, 2) \
	x(USER  , 4) \

#define LOG_X(n, v) n = v,
enum class LogCategory : unsigned
{
  LOG_MAP(LOG_X)
};
#undef LOG_X
using LogCategoryType = std::underlying_type_t<LogCategory>;

#define LOG_X(n, v) + 1
constexpr size_t log_category_count = 0 + LOG_MAP(LOG_X);
#undef LOG_X

#define LOG_X(n, v) if(idx == i) { return static_cast<LogCategory>(v); } ++ i;
constexpr LogCategory LogCategoryFromIdx(size_t idx)
{
  size_t i = 0;
  LOG_MAP(LOG_X)
  return static_cast<LogCategory>(0);
}
#undef LOG_X
#define LOG_X(n, v) if(static_cast<LogCategory>(v) == category) { return i; } ++i;
constexpr size_t LogCategoryIdx(LogCategory category)
{
  size_t i = 0;
  LOG_MAP(LOG_X)
  return static_cast<size_t>(-1);
}
#undef LOG_X

inline LogCategory operator|(LogCategory a, LogCategory b)
{
  auto ua = static_cast<LogCategoryType>(a);
  auto ub = static_cast<LogCategoryType>(b);
  return static_cast<LogCategory>(ua | ub);
}
inline LogCategory operator&(LogCategory a, LogCategory b)
{
  auto ua = static_cast<LogCategoryType>(a);
  auto ub = static_cast<LogCategoryType>(b);
  return static_cast<LogCategory>(ua & ub);
}
inline bool operator&&(LogCategory a, LogCategory b)
{
  auto ua = static_cast<LogCategoryType>(a);
  auto ub = static_cast<LogCategoryType>(b);
  return (ua & ub) != 0;
}

// ****************************************************************************
#define MAX_LOG_STRINGS 256

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// ****************************************************************************
class Logger
{
protected:
  Logger()
  {}

  std::ofstream log_streams[log_category_count];
  String log_strings[MAX_LOG_STRINGS];

#ifdef WIN32
  bool LoadStrings()
  {
    char buff[1024];

    auto module_handle = GetModuleHandle(NULL);
    for(UINT i = 1; ; ++i)
    {
      if(0 == LoadStringA(module_handle, i, buff, _countof(buff)))
      {
        break;
      }

      log_strings[i] = buff;
    }

    return true;
  }
#else
  bool LoadStrings()
  {
    std::ifstream in("strings.txt");
    if(!in.is_open())
    {
      return false;
    }

    size_t idx = 0;
    while(!in.eof())
    {
      char str_buff[1024];
      in.getline(str_buff, 1024);
      log_strings[idx++] = str_buff;
    }

    return true;
  }
#endif

public:
  static Logger& Get()
  {
    static Logger instance;
    return instance;
  }

  bool Init()
  {
    log_streams[LogCategoryIdx(LogCategory::GAME)].open("GameLog.txt");
    log_streams[LogCategoryIdx(LogCategory::ENGINE)].open("EngineLog.txt");

    if(!LoadStrings())
    {
      return false;
    }

    return true;
  }

  void Write(LogCategory categories, StringRef msg, ...)
  {
    va_list args; va_start(args, msg);

    char msg_buff[1024];
    vsprintf_s(msg_buff, msg.data(), args);

    for(size_t i = 0; i < log_category_count; ++i)
    {
      auto target_category = LogCategoryFromIdx(i);
      if(categories && target_category)
      {
        if((categories & target_category) == LogCategory::USER)
        {
#ifdef WIN32
          MessageBoxA(NULL, msg_buff, "Message", MB_OK);
#else
#error User-level logging is not implemented for this platform.
#endif
        }
        else
        {
          auto& str = log_streams[i];
          str << msg_buff;
          str << '\n';
#ifdef _DEBUG
          str.flush();
#endif
        }
      }
    }
  }
  void Write(LogCategory categories, size_t msg_id, ...)
  {
    va_list args; va_start(args, msg_id);

    char msg_buff[1024];
    vsprintf_s(msg_buff, log_strings[msg_id].c_str(), args);

    Write(categories, msg_buff);
  }

};
