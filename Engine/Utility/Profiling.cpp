#include "Profiling.h"

#include <cassert>

// ****************************************************************************
ProfilerOutputHandler* ProfileSample::output_handler = nullptr;
int ProfileSample::last_opened_sample = -1;
int ProfileSample::open_sample_count = 0;
float ProfileSample::root_begin = 0;
float ProfileSample::root_end = 0;
ProfileSample::Sample ProfileSample::samples[MAX_PROFILER_SAMPLES];

// ****************************************************************************
ProfileSample::ProfileSample(StringRef tag)
{
  int store_index = -1;
  for(int i = 0; i < MAX_PROFILER_SAMPLES; ++i)
  {
    Sample& curr_sample = samples[i];
    if(!curr_sample.is_valid)
    {
      if(store_index == -1)
      {
        store_index = i;
      }
    }
    else
    {
      if(curr_sample.tag == tag)
      {
        assert(!curr_sample.is_open && "Duplicate profiler sample tag found");

        sample_index = i;
        parent_index = last_opened_sample;
        last_opened_sample = i;

        curr_sample.parent_count = open_sample_count;
        ++open_sample_count;
        curr_sample.is_open = true;

        ++curr_sample.call_count;

        curr_sample.start_time = GetTime();

        if(parent_index == -1)
        {
          root_begin = curr_sample.start_time;
        }

        return;
      }
    }
  }

  assert(store_index != -1 && "Profiler has run out of sample slots");

  Sample& new_sample = samples[store_index];
  new_sample.is_valid = true;
  new_sample.tag = tag;

  sample_index = store_index;
  parent_index = last_opened_sample;
  last_opened_sample = store_index;

  new_sample.parent_count = open_sample_count;
  ++open_sample_count;
  new_sample.is_open = true;
  new_sample.call_count = 1;

  new_sample.total_time = 0.f;
  new_sample.child_time = 0.f;
  new_sample.start_time = GetTime();

  if(parent_index == -1)
  {
    root_begin = new_sample.start_time;
  }
}

ProfileSample::~ProfileSample()
{
  float end_time = GetTime();

  Sample& my_sample = samples[sample_index];
  my_sample.is_open = false;
  float time_taken = end_time - my_sample.start_time;

  if(parent_index != -1)
  {
    samples[parent_index].child_time += time_taken;
  }
  else
  {
    root_end = end_time;
  }

  my_sample.total_time += time_taken;
  last_opened_sample = parent_index;
  --open_sample_count;
}

// ****************************************************************************
void ProfileSample::Output()
{
  assert(output_handler && "Profiler has not output handler set");

  output_handler->BeginOutput();

  for(int i = 0; i < MAX_PROFILER_SAMPLES; ++i)
  {
    Sample& curr_sample = samples[i];

    if(curr_sample.is_valid)
    {
      float sample_time, percentage;

      // calculate percentage time spent on the sample itself (excluding children)
      sample_time = curr_sample.total_time;// -curr_sample._child_time;
      percentage = (sample_time / (root_end - root_begin)) * 100.f;

      // add and recalculate average
      float total_pc;
      total_pc = curr_sample.average_pc * curr_sample.data_count;
      total_pc += percentage;
      ++curr_sample.data_count;
      curr_sample.average_pc = total_pc / curr_sample.data_count;

      // update min/max values
      if(curr_sample.min_pc == -1.f || percentage < curr_sample.min_pc)
      {
        curr_sample.min_pc = percentage;
      }
      if(curr_sample.max_pc == -1.f || percentage > curr_sample.max_pc)
      {
        curr_sample.max_pc = percentage;
      }

      output_handler->ShowSample(
        curr_sample.min_pc,
        curr_sample.average_pc,
        curr_sample.max_pc,
        curr_sample.call_count,
        curr_sample.tag,
        curr_sample.parent_count
      );

      // reset sample for next use
      curr_sample.call_count = 0;
      curr_sample.total_time = 0;
      curr_sample.child_time = 0;
    }
  }

  output_handler->EndOutput();
}
void ProfileSample::ResetSample(StringRef tag)
{
  for(int i = 0; i < MAX_PROFILER_SAMPLES; ++i)
  {
    Sample& curr_sample = samples[i];
    if(curr_sample.is_valid)
    {
      if(curr_sample.tag == tag)
      {
        curr_sample.is_valid = false;
      }
    }
  }
}
void ProfileSample::ResetAll()
{
  for(int i = 0; i < MAX_PROFILER_SAMPLES; ++i)
  {
    samples[i].is_valid = false;
  }
}