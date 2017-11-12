#pragma once
class Arena;

class ArenaBaseObject
{
  friend class Arena;
  template<typename T>
  friend class Handle;
protected:
  static Arena* arena;

};
