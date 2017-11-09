#pragma once
#include <chrono>

class Timer
{
  typedef std::chrono::high_resolution_clock HighResolutionClock;
  typedef std::chrono::milliseconds MiliSeconds;
  typedef std::chrono::duration<double> FloatSeconds;

public:
  Timer()
  {
    Reset();
  }
  void Reset()
  {
    start = HighResolutionClock::now();
  }
  double Elapsed() const
  {
    return std::chrono::duration_cast<FloatSeconds>(HighResolutionClock::now() - start).count();
  }
  static double Time()
  {
    return std::chrono::duration_cast<FloatSeconds>(HighResolutionClock::now() - program_start).count();
  }
private:
  static HighResolutionClock::time_point program_start;
  HighResolutionClock::time_point start;
};
