#pragma once

template<typename It, typename T>
inline It Find(It begin, It end, const T& val)
{
  for(; begin != end; begin++)
  {
    if(*begin == val)
    {
      break;
    }
  }
  return begin;
}
template<typename It, typename Pr, typename... Args>
inline It FindByPred(It begin, It end, Pr pred, Args&... args)
{
  for(; begin != end; begin++)
  {
    if(pred(*begin, args...))
    {
      break;
    }
  }
  return begin;
}