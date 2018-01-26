#pragma once
#include "Singleton.h"

static constexpr size_t FRAME_ALLOC_BUFF_SIZE = 256 * 1024 * 1024; // 32 megabytes should be enough
class FrameAllocator : public Singleton<FrameAllocator>
{
public:
  FrameAllocator();
  ~FrameAllocator();

  void Clear();

  template<typename T>
  T* Allocate(size_t size)
  {
    T* res = (T*)cur_offset;
    cur_offset += size * sizeof(T);

	assert(cur_offset < (buff + FRAME_ALLOC_BUFF_SIZE) 
		&&	"Frame Allocation exceeded buffer size");

    return res;
  }

private:
  unsigned char* buff;
  unsigned char* cur_offset;
};
