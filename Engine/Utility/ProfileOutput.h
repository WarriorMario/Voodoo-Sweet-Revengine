#pragma once

#include "Utility/Profiling.h"
#include "Utility/Logging.h"

// ****************************************************************************
#define PROFILE_HEADER1 "Min : Avg : Max : Ms : # : Profile tag"
#define PROFILE_HEADER2 "---------------------------------"
#define PROFILE_SAMPLE "%s : %s  : %s : %s : %s : %s"

// ****************************************************************************
class ProfilerLogHandler : public ProfilerOutputHandler
{
  void BeginOutput() override;
  void ShowSample(ProfileSample::Sample& sample) override;
  void EndOutput() override;

};
void ProfilerLogHandler::BeginOutput()
{
  Logger::Get().Write(LogCategory::ENGINE, PROFILE_HEADER1);
  Logger::Get().Write(LogCategory::ENGINE, PROFILE_HEADER2);
}
void ProfilerLogHandler::EndOutput()
{
  Logger::Get().Write(LogCategory::ENGINE, "\n");
}
void ProfilerLogHandler::ShowSample(ProfileSample::Sample& sample)
{
  char avg[16];
  char min[16];
  char max[16];
  char ms[16];
  char num[16];

  sprintf_s(avg, "%5.1f", sample.average_pc);
  sprintf_s(min, "%5.1f", sample.min_pc);
  sprintf_s(max, "%5.1f", sample.max_pc);
  sprintf_s(ms, "%5.1f", sample.total_time * 1000.0f);
  sprintf_s(num, "%3d", sample.call_count);

  char indented_tag[256] = "";
  assert(sample.parent_count < CountOf(indented_tag) - 1);
  for(int indent = 0; indent < sample.parent_count; ++indent)
  {
    indented_tag[indent] = ' ';
    indented_tag[indent + 1] = '\0';
  }
  strcat_s(indented_tag, sample.tag.data());

  Logger::Get().Write(LogCategory::ENGINE, PROFILE_SAMPLE, min, avg, max, ms, num, indented_tag);
}