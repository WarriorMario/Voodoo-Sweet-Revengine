#pragma once

#include "Utility/Profiling.h"
#include "Utility/Logging.h"

// ****************************************************************************
#define PROFILE_HEADER1 "Min : Avg : Max : # : Profile tag"
#define PROFILE_HEADER2 "---------------------------------"
#define PROFILE_SAMPLE "%s : %s  : %s : %s : %s"

// ****************************************************************************
class ProfilerLogHandler : public ProfilerOutputHandler
{
  void BeginOutput() override;
  void ShowSample(float min_pc, float avg_pc, float max_pc, int call_count, std::string_view tag, int parent_count) override;
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
void ProfilerLogHandler::ShowSample(float min_pc, float avg_pc, float max_pc, int call_count, std::string_view tag, int parent_count)
{
  char avg[16];
  char min[16];
  char max[16];
  char num[16];

  sprintf_s(avg, "%5.1f", avg_pc);
  sprintf_s(min, "%5.1f", min_pc);
  sprintf_s(max, "%5.1f", max_pc);
  sprintf_s(num, "%3d", call_count);

  char tag_buf[256];
  char indented_tag[256];

  strcpy_s(indented_tag, tag.data());
  for(int indent = 0; indent < parent_count; ++indent)
  {
    sprintf_s(tag_buf, " %s", indented_tag);
    strcpy_s(indented_tag, tag_buf);
  }

  Logger::Get().Write(LogCategory::ENGINE, PROFILE_SAMPLE, min, avg, max, num, indented_tag);
}