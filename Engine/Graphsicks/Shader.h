#pragma once

template<typename T>
class Shader
{
  void Shade(T::PrimData&, T::ConstData&);
};

struct BG
{
  struct PrimData
  {

  };

  struct ConstData
  {

  };
};

template<>
void Shader<BG>::Shade(BG::PrimData& primData, BG::ConstData& constData)
{

}