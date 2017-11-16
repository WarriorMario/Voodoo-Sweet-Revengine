#include "ScreenGrid.h"
#include "Graphics.h"

void ThreadedUnPackBuffer(void* data)
{
	UnPackBufferData* real_data = (UnPackBufferData*)data;
  for(int y = 0; y < ScreenGrid::CELL_HEIGHT; ++y)
  {
    Color* cur = &real_data->destination[((real_data->y) * ScreenGrid::CELL_HEIGHT+y)*Graphics::ScreenWidth];
    for(int j = 0; j < ScreenGrid::WIDTH; ++j)
    {
      ScreenGridCell& cell = real_data->cell_line_start[j];

      memcpy(cur, &cell.buff[y*ScreenGrid::CELL_WIDTH], sizeof(Color)*ScreenGrid::CELL_WIDTH);
      cur +=ScreenGrid::CELL_WIDTH;
    }
	}
}