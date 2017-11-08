/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "Mat3.h"
#include "Rect.h"
#include "Utility.h"

template<int width, int height, int cell_width = 16, int cell_height = 16>
class Grid
{
  constexpr size_t RESOLUTION_X = width * cell_width;
  constexpr size_t RESOLUTION_Y = height * cell_height;

public:
  struct Cell
  {
    Color* buff;
  };
  Grid()
  {
    static_assert(IsPowerOf2(cell_width));
    static_assert(IsPowerOf2(cell_height));

    buff = new Color[RESOLUTION_X * RESOLUTION_Y];
    int cell_buff_sz = cell_width * cell_height;
    int offset = 0;
    for(int i = 0; i < height; ++i)
    {
      for(int j = 0; j < width; ++j, offset += cell_buff_sz)
      {
        cells[i * height + j].buff = buff + offset;
      }
    }
  }
  ~Grid()
  {
    delete[] buff;
  }

private:
  Cell cells[width * height];
  Color* buff;
};



class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key, HWNDKey& key2 );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame();
	void PutPixel( int x,int y,Color c );
   void DrawLine( Vec2 p0, Vec2 p1, Color c );
   inline void DrawLine(int x1, int y1, int x2, int y2, Color c);
   inline void DrawLineClip(Vec2 p0, Vec2 p1, Color color, const RectF& clip);
   void DrawClippedLineCircle(float size, Vec2 pos, unsigned int points, const RectF& clip);
	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;

   Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain2;
   Microsoft::WRL::ComPtr<ID3D11Device>				pDevice2;
   Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext2;
   Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView2;
   Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture2;
   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView2;
   Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader2;
   Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader2;
   Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer2;
   Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout2;
   Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState2;
   D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture2;
   Color*                                              pSysBuffer2 = nullptr;
   float*                                              pZBuffer2 = nullptr;

public:
	static constexpr int ScreenWidth = 800;
	static constexpr int ScreenHeight = 600;
};