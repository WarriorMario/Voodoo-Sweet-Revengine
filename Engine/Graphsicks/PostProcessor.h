#pragma once

class Grid;

class PostProcessor
{
public:
  virtual void PostProcess(Grid& grid) = 0;
};