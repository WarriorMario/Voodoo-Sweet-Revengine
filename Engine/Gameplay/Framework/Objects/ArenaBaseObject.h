#pragma once
class Arena;

class ArenaBaseObject
{
  friend class Arena;
  template<typename T>
  friend class Handle;
  friend class GodEditor;
protected:
  static Arena* arena;
  static class Input* input;

};
