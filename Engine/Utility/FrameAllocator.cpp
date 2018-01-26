#include "FrameAllocator.h"
#include <memory>

FrameAllocator::FrameAllocator()
{
  buff = (unsigned char*)malloc(FRAME_ALLOC_BUFF_SIZE);
  cur_offset = buff;
  memset(buff, 0, FRAME_ALLOC_BUFF_SIZE);
}

FrameAllocator::~FrameAllocator()
{
	free(buff);
	buff = cur_offset = nullptr;
}

void FrameAllocator::Clear()
{
  cur_offset = buff;
  //memset(buff, 0, FRAME_ALLOC_BUFF_SIZE);
}
