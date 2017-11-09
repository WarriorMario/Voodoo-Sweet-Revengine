#pragma once

#include "VString.h"
#include "Timer.h"

#define MAX_PROFILER_SAMPLES 1024

// ****************************************************************************
class ProfilerOutputHandler
{
public:
  virtual void BeginOutput() = 0;
  virtual void ShowSample(float min_pc, float avg_pc, float max_pc, int call_count, StringRef tag, int parent_count) = 0;
  virtual void EndOutput() = 0;

};

// ****************************************************************************
class ProfileSample
{
public:
  ProfileSample(StringRef tag);
  ~ProfileSample();

  static void Output();
  static void ResetSample(StringRef tag);
  static void ResetAll();
  static ProfilerOutputHandler* output_handler;

protected:
  int sample_index;
  int parent_index;

  static Timer timer;

  float GetTime()
  {
    return (float)Timer::Time() / 1000.f;
  }

  static struct Sample
  {
    Sample()
      : is_valid(false)
      , data_count(0)
      , average_pc(-1.f)
      , min_pc(-1.f)
      , max_pc(-1.f)
    {}

    bool is_valid;
    bool is_open;
    unsigned int call_count;
    String tag;

    float start_time;
    float total_time;
    float child_time;

    int parent_count;

    float average_pc;
    float min_pc;
    float max_pc;
    size_t data_count; // number of times values have been stored since creation/reset
  } samples[MAX_PROFILER_SAMPLES];

  static int last_opened_sample;
  static int open_sample_count;
  static float root_begin, root_end;

};

// ****************************************************************************
#define PROFILE_SCOPE(tag) ProfileSample Sample##__LINE__(tag)
