#include "ResourceAllocator.h"
#include <memory>

ResourceAllocator::ResourceAllocator()
{
  buff = (unsigned char*)malloc(RESOURCE_ALLOC_BUFF_SIZE);
  cur_offset = buff;
  memset(buff, 0, RESOURCE_ALLOC_BUFF_SIZE);
}

ResourceAllocator::~ResourceAllocator()
{
  free(buff);
  buff = cur_offset = nullptr;
}

void ResourceAllocator::Clear()
{
  cur_offset = buff;
  //memset(buff, 0, FRAME_ALLOC_BUFF_SIZE);
}
