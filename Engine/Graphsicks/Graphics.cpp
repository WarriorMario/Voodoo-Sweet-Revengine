/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.cpp																		  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
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
#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"
#include <assert.h>
#include <string>
#include <array>
#include <algorithm>

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )

#define CHILI_GFX_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

using Microsoft::WRL::ComPtr;

Graphics::Graphics( HWNDKey& key, HWNDKey& key2 )
{
	assert( key.hWnd != nullptr );
   assert( key2.hWnd != nullptr );

	//////////////////////////////////////////////////////
	// create device and swap chain/get render target view
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Graphics::ScreenWidth;
	sd.BufferDesc.Height = Graphics::ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = key.hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

   //////////////////////////////////////////////////////
   // create second device and swap chain/get render target view
   DXGI_SWAP_CHAIN_DESC sd2 = {};
   sd2.BufferCount = 1;
   sd2.BufferDesc.Width = Graphics::ScreenWidth;
   sd2.BufferDesc.Height = Graphics::ScreenHeight;
   sd2.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
   sd2.BufferDesc.RefreshRate.Numerator = 1;
   sd2.BufferDesc.RefreshRate.Denominator = 60;
   sd2.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   sd2.OutputWindow = key2.hWnd;
   sd2.SampleDesc.Count = 1;
   sd2.SampleDesc.Quality = 0;
   sd2.Windowed = TRUE;

	HRESULT				hr;
	UINT				createFlags = 0u;
#ifdef CHILI_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif
	
	// create device and front/back buffers
	if( FAILED( hr = D3D11CreateDeviceAndSwapChain( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating device and swap chain" );
	}

   // create device and front/back buffers
   if ( FAILED( hr = D3D11CreateDeviceAndSwapChain(
      nullptr,
      D3D_DRIVER_TYPE_HARDWARE,
      nullptr,
      createFlags,
      nullptr,
      0,
      D3D11_SDK_VERSION,
      &sd2,
      &pSwapChain2,
      &pDevice2,
      nullptr,
      &pImmediateContext2 ) ) )
   {
      throw CHILI_GFX_EXCEPTION( hr, L"Creating device and swap chain" );
   }

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if( FAILED( hr = pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),
		(LPVOID*)&pBackBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Getting back buffer" );
	}
   // get handle to backbuffer
   ComPtr<ID3D11Resource> pBackBuffer2;
   if ( FAILED( hr = pSwapChain2->GetBuffer(
      0,
      __uuidof( ID3D11Texture2D ),
      ( LPVOID* )&pBackBuffer2 ) ) )
   {
      throw CHILI_GFX_EXCEPTION( hr, L"Getting back buffer" );
   }


	// create a view on backbuffer that we can render to
	if( FAILED( hr = pDevice->CreateRenderTargetView( 
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating render target view on backbuffer" );
	}
   // create a view on backbuffer that we can render to
   if ( FAILED( hr = pDevice2->CreateRenderTargetView(
      pBackBuffer2.Get(),
      nullptr,
      &pRenderTargetView2 ) ) )
   {
      throw CHILI_GFX_EXCEPTION( hr, L"Creating render target view on backbuffer" );
   }


	// set backbuffer as the render target using created view
	pImmediateContext->OMSetRenderTargets( 1,pRenderTargetView.GetAddressOf(),nullptr );

   // set backbuffer as the render target using created view
   pImmediateContext2->OMSetRenderTargets( 1, pRenderTargetView2.GetAddressOf(), nullptr );

	// set viewport dimensions
	D3D11_VIEWPORT vp;
	vp.Width = float( Graphics::ScreenWidth );
	vp.Height = float( Graphics::ScreenHeight );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports( 1,&vp );
   pImmediateContext2->RSSetViewports( 1, &vp );


	///////////////////////////////////////
	// create texture for cpu render target
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = Graphics::ScreenWidth;
	sysTexDesc.Height = Graphics::ScreenHeight;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	if( FAILED( hr = pDevice->CreateTexture2D( &sysTexDesc,nullptr,&pSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sysbuffer texture" );
	}
   // create the texture
   if ( FAILED( hr = pDevice2->CreateTexture2D( &sysTexDesc, nullptr, &pSysBufferTexture2 ) ) )
   {
      throw CHILI_GFX_EXCEPTION( hr, L"Creating sysbuffer texture" );
   }

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// create the resource view on the texture
	if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
		&srvDesc,&pSysBufferTextureView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating view on sysBuffer texture" );
	}
   // create the resource view on the texture
   if ( FAILED( hr = pDevice2->CreateShaderResourceView( pSysBufferTexture2.Get(),
      &srvDesc, &pSysBufferTextureView2 ) ) )
   {
      throw CHILI_GFX_EXCEPTION( hr, L"Creating view on sysBuffer texture" );
   }

	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof( FramebufferShaders::FramebufferPSBytecode ),
		nullptr,
		&pPixelShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating pixel shader" );
	}
   ////////////////////////////////////////////////
   // create pixel shader for framebuffer
   // Ignore the intellisense error "namespace has no member"
   if ( FAILED( hr = pDevice2->CreatePixelShader(
      FramebufferShaders::FramebufferPSBytecode,
      sizeof( FramebufferShaders::FramebufferPSBytecode ),
      nullptr,
      &pPixelShader2 ) ) )
   {
      throw CHILI_GFX_EXCEPTION( hr, L"Creating pixel shader" );
   }

	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateVertexShader(
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		nullptr,
		&pVertexShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex shader" );
	}
	
   /////////////////////////////////////////////////
   // create vertex shader for framebuffer
   // Ignore the intellisense error "namespace has no member"
   if ( FAILED( hr = pDevice2->CreateVertexShader(
      FramebufferShaders::FramebufferVSBytecode,
      sizeof( FramebufferShaders::FramebufferVSBytecode ),
      nullptr,
      &pVertexShader2 ) ) )
   {
      throw CHILI_GFX_EXCEPTION( hr, L"Creating vertex shader" );
   }

	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( FSQVertex ) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if( FAILED( hr = pDevice->CreateBuffer( &bd,&initData,&pVertexBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex buffer" );
	}
   if ( FAILED( hr = pDevice2->CreateBuffer( &bd, &initData, &pVertexBuffer2 ) ) )
   {
      throw CHILI_GFX_EXCEPTION( hr, L"Creating vertex buffer" );
   }

	
	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateInputLayout( ied,2,
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		&pInputLayout ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating input layout" );
	}
   // Ignore the intellisense error "namespace has no member"
   if ( FAILED( hr = pDevice2->CreateInputLayout( ied, 2,
      FramebufferShaders::FramebufferVSBytecode,
      sizeof( FramebufferShaders::FramebufferVSBytecode ),
      &pInputLayout2 ) ) )
   {
      throw CHILI_GFX_EXCEPTION( hr, L"Creating input layout" );
   }

	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if( FAILED( hr = pDevice->CreateSamplerState( &sampDesc,&pSamplerState ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sampler state" );
	}
   if ( FAILED( hr = pDevice2->CreateSamplerState( &sampDesc, &pSamplerState2 ) ) )
   {
      throw CHILI_GFX_EXCEPTION( hr, L"Creating sampler state" );
   }

	// allocate memory for sysbuffer (16-byte aligned for faster access)
	god_window_buffer = reinterpret_cast<Color*>( 
		_aligned_malloc( sizeof( Color ) * Graphics::ScreenWidth * Graphics::ScreenHeight,16u ) );
   // allocate memory for sysbuffer (16-byte aligned for faster access)
   player_window_buffer = reinterpret_cast<Color*>(
      _aligned_malloc( sizeof( Color ) * Graphics::ScreenWidth * Graphics::ScreenHeight, 16u ) );
}

Graphics::~Graphics()
{
	// free sysbuffer memory (aligned free)
	if( god_window_buffer )
	{
		_aligned_free( god_window_buffer );
		god_window_buffer = nullptr;
	}
	// clear the state of the device context before destruction
	if( pImmediateContext ) pImmediateContext->ClearState();

   // free sysbuffer memory (aligned free)
   if ( player_window_buffer )
   {
      _aligned_free( player_window_buffer );
      player_window_buffer = nullptr;
   }
   // clear the state of the device context before destruction
   if ( pImmediateContext2 ) pImmediateContext2->ClearState();
}

void Graphics::EndFrame()
{
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(),0u,
		D3D11_MAP_WRITE_DISCARD,0u,&mappedSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Mapping sysbuffer" );
	}
	// setup parameters for copy operation
	Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData );
	 size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof( Color );
	 size_t srcPitch = Graphics::ScreenWidth;
	 size_t rowBytes = srcPitch * sizeof( Color );
	// perform the copy line-by-line
	for( size_t y = 0u; y < Graphics::ScreenHeight; y++ )
	{
		memcpy( &pDst[ y * dstPitch ],&god_window_buffer[y * srcPitch],rowBytes );
	}
	// release the adapter memory
	pImmediateContext->Unmap( pSysBufferTexture.Get(),0u );

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout( pInputLayout.Get() );
	pImmediateContext->VSSetShader( pVertexShader.Get(),nullptr,0u );
	pImmediateContext->PSSetShader( pPixelShader.Get(),nullptr,0u );
	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	UINT stride = sizeof( FSQVertex );
	UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
	pImmediateContext->PSSetShaderResources( 0u,1u,pSysBufferTextureView.GetAddressOf() );
	pImmediateContext->PSSetSamplers( 0u,1u,pSamplerState.GetAddressOf() );
	pImmediateContext->Draw( 6u,0u );


	// flip back/front buffers
	if( FAILED( hr = pSwapChain->Present( 1u,0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw CHILI_GFX_EXCEPTION( pDevice->GetDeviceRemovedReason(),L"Presenting back buffer [device removed]" );
		}
		else
		{
			throw CHILI_GFX_EXCEPTION( hr,L"Presenting back buffer" );
		}
	}


   // lock and map the adapter memory for copying over the sysbuffer
   if ( FAILED( hr = pImmediateContext2->Map( pSysBufferTexture2.Get(), 0u,
      D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture2 ) ) )
   {
      throw CHILI_GFX_EXCEPTION( hr, L"Mapping sysbuffer" );
   }
   // setup parameters for copy operation
   pDst = reinterpret_cast<Color*>( mappedSysBufferTexture2.pData );
   dstPitch = mappedSysBufferTexture2.RowPitch / sizeof( Color );
   srcPitch = Graphics::ScreenWidth;
   rowBytes = srcPitch * sizeof( Color );
   // perform the copy line-by-line
   for ( size_t y = 0u; y < Graphics::ScreenHeight; y++ )
   {
      memcpy( &pDst[y * dstPitch], &player_window_buffer[y * srcPitch], rowBytes );
   }
   // release the adapter memory
   pImmediateContext2->Unmap( pSysBufferTexture2.Get(), 0u );

   // render offscreen scene texture to back buffer
   pImmediateContext2->IASetInputLayout( pInputLayout2.Get() );
   pImmediateContext2->VSSetShader( pVertexShader2.Get(), nullptr, 0u );
   pImmediateContext2->PSSetShader( pPixelShader2.Get(), nullptr, 0u );
   pImmediateContext2->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
   stride = sizeof( FSQVertex );
   offset = 0u;
   pImmediateContext2->IASetVertexBuffers( 0u, 1u, pVertexBuffer2.GetAddressOf(), &stride, &offset );
   pImmediateContext2->PSSetShaderResources( 0u, 1u, pSysBufferTextureView2.GetAddressOf() );
   pImmediateContext2->PSSetSamplers( 0u, 1u, pSamplerState2.GetAddressOf() );
   pImmediateContext2->Draw( 6u, 0u );
   // flip back/front buffers
   if ( FAILED( hr = pSwapChain2->Present( 1u, 0u ) ) )
   {
      if ( hr == DXGI_ERROR_DEVICE_REMOVED )
      {
         throw CHILI_GFX_EXCEPTION( pDevice2->GetDeviceRemovedReason(), L"Presenting back buffer [device removed]" );
      }
      else
      {
         throw CHILI_GFX_EXCEPTION( hr, L"Presenting back buffer" );
      }
   }
}

void Graphics::BeginFrame()
{
	// clear the sysbuffer
	memset( god_window_buffer,0u,sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
   // clear the sysbuffer
   memset( player_window_buffer, 0u, sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
}

void Graphics::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	god_window_buffer[Graphics::ScreenWidth * y + x] = c;
   player_window_buffer[Graphics::ScreenWidth * y + x] = Colors::Cyan;
}

void Graphics::DrawLine( Vec2 p0, Vec2 p1, Color c )
{
   if( p0.y > p1.y )
   {
      b2Swap( p0.y, p1.y );
   }
   float y0 = p0.y;
   float y1 = p1.y;
   float x0 = p0.x;
   float x1 = p1.x;
   float dx = ( p1.x - p0.x ) / ( p1.y - p0.y );
   for( int i = (int)y0; i < (int)y1; ++i )
   {
      PutPixel( (int)x0, i, Colors::White );
      x0 += dx;
   }
}
inline void Graphics::DrawLine(int x1, int y1, int x2, int y2, Color c)
{

	const int dx = x2 - x1;
	const int dy = y2 - y1;

	if (dy == 0 && dx == 0)
	{
		PutPixel(x1, y1, c);
	}
	else if (abs(dy) > abs(dx))
	{
		if (dy < 0)
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		const float m = (float)dx / (float)dy;
		const float b = x1 - m*y1;
		for (int y = y1; y <= y2; y = y + 1)
		{
			int x = (int)(m*y + b + 0.5f);
			PutPixel(x, y, c);
		}
	}
	else
	{
		if (dx < 0)
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		const float m = (float)dy / (float)dx;
		const float b = y1 - m*x1;
		for (int x = x1; x <= x2; x = x + 1)
		{
			int y = (int)(m*x + b + 0.5f);
			PutPixel(x, y, c);
		}
	}
}
void Graphics::DrawClippedLineCircle(float size, Vec2 pos, unsigned int points, const RectF & clip)
{
	float stepSize = b2_pi * 2 / points;
	unsigned int i = 0;
	for (; i < points - 1; ++i)
	{
		DrawLineClip(pos + Vec2{ size*cos(i*stepSize), size*sin(i*stepSize) }, pos + Vec2{ size*cos((i + 1) * stepSize), size*sin((i + 1) * stepSize) }, Colors::White, clip);
	}
	DrawLineClip(pos + Vec2{ size*cos(i*stepSize), size*sin(i*stepSize) }, pos + Vec2{ size*cos(0.0f), size*sin(0.0f) }, Colors::Blue, clip);
}
void Graphics::DrawLineClip(Vec2 p0, Vec2 p1, Color color, const RectF& clip)
{
	enum OutCode
	{
		INSIDE = 0, // 0000
		LEFT = 1,   // 0001
		RIGHT = 2,  // 0010
		BOTTOM = 4, // 0100
		TOP = 8     // 1000
	};

	auto ComputeOutCode =
		[&clip](float x, float y) -> OutCode
	{
		OutCode code = INSIDE;   // initialised as being inside of clip window

		if (x < clip.left)           // to the left of clip window
			code = (OutCode)(code | LEFT);
		else if (x > clip.right)      // to the right of clip window
			code = (OutCode)(code | RIGHT);
		if (y < clip.top)           // below the clip window
			code = (OutCode)(code | BOTTOM);
		else if (y > clip.bottom)      // above the clip window
			code = (OutCode)(code | TOP);

		return code;
	};

	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	OutCode outcode0 = ComputeOutCode(p0.x, p0.y);
	OutCode outcode1 = ComputeOutCode(p1.x, p1.y);
	bool accept = false;

	while (true)
	{
		if (!(outcode0 | outcode1))
		{ // Bitwise OR is 0. Trivially accept and get out of loop
			accept = true;
			break;
		}
		else if (outcode0 & outcode1)
		{ // Bitwise AND is not 0. Trivially reject and get out of loop
			break;
		}
		else
		{
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			float x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find the intersection point;
			// use formulas y = p0.y + slope * (x - p0.x), x = p0.x + (1 / slope) * (y - p0.y)
			if (outcodeOut & TOP)
			{           // point is above the clip rectangle
				x = p0.x + (p1.x - p0.x) * (clip.bottom - p0.y) / (p1.y - p0.y);
				y = clip.bottom;
			}
			else if (outcodeOut & BOTTOM)
			{ // point is below the clip rectangle
				x = p0.x + (p1.x - p0.x) * (clip.top - p0.y) / (p1.y - p0.y);
				y = clip.top;
			}
			else if (outcodeOut & RIGHT)
			{  // point is to the right of clip rectangle
				y = p0.y + (p1.y - p0.y) * (clip.right - p0.x) / (p1.x - p0.x);
				x = clip.right;
			}
			else if (outcodeOut & LEFT)
			{   // point is to the left of clip rectangle
				y = p0.y + (p1.y - p0.y) * (clip.left - p0.x) / (p1.x - p0.x);
				x = clip.left;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0)
			{
				p0.x = x;
				p0.y = y;
				outcode0 = ComputeOutCode(p0.x, p0.y);
			}
			else
			{
				p1.x = x;
				p1.y = y;
				outcode1 = ComputeOutCode(p1.x, p1.y);
			}
		}
	}
	if (accept)
	{
		DrawLine((int)(p0.x + 0.5), (int)(p0.y + 0.5), (int)(p1.x + 0.5), (int)(p1.y + 0.5), color);
	}
}


//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line )
	:
	ChiliException( file,line,note ),
	hr( hr )
{}

std::wstring Graphics::Exception::GetFullMessage() const
{
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring& note = GetNote();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
		: empty)
		+ (!errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
			: empty)
		+ (!note.empty() ? std::wstring( L"Note: " ) + note + L"\n"
			: empty)
		+ (!location.empty() ? std::wstring( L"Location: " ) + location
			: empty);
}

std::wstring Graphics::Exception::GetErrorName() const
{
	return DXGetErrorString( hr );
}

std::wstring Graphics::Exception::GetErrorDescription() const
{
	std::array<wchar_t,512> wideDescription;
	DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
	return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const
{
	return L"Chili Graphics Exception";
}