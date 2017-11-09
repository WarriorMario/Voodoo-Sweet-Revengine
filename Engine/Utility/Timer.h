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
private:
  HighResolutionClock::time_point start;
};