/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	MainWindow.h																		  *
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
#include "ChiliWin.h"
#include "Graphsicks\Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ChiliException.h"
#include <string>

// for granting special access to hWnd only for Graphics constructor
class HWNDKey
{
	friend Graphics::Graphics( HWNDKey&, HWNDKey& );
public:
	HWNDKey( const HWNDKey& ) = delete;
	HWNDKey& operator=( HWNDKey& ) = delete;
protected:
	HWNDKey() = default;
protected:
	HWND hWnd = nullptr;
};

class RenderWindow : public HWNDKey
{
public:
   class Exception : public ChiliException
   {
   public:
      using ChiliException::ChiliException;
      virtual std::wstring GetFullMessage() const override { return GetNote() + L"\nAt: " + GetLocation(); }
      virtual std::wstring GetExceptionType() const override { return L"Windows Exception"; }
   };
public:
   RenderWindow( HINSTANCE hInst, wchar_t* pArgs, wchar_t* name = L"Voodoo Sweet Revengine Rendering Window", WNDPROC wndProc = DefWindowProc );
   RenderWindow( const RenderWindow& ) = delete;
   RenderWindow& operator=( const RenderWindow& ) = delete;
   ~RenderWindow();
   
   bool IsActive() const;
   void SetFocused();
   bool IsFocused() const;
   bool IsMinimized() const;
   void ShowMessageBox( const std::wstring& title, const std::wstring& message ) const;

   static bool IsOpen();
   static void Kill();

   const std::wstring& GetArgs() const
   {
      return args;
   }

protected:
   HINSTANCE hInst = nullptr;
   std::wstring args;
   wchar_t* wndClassName;

   static bool isOpen;
};

class MainWindow : public RenderWindow
{
public:
   class Exception : public ChiliException
   {
   public:
      using ChiliException::ChiliException;
      virtual std::wstring GetFullMessage() const override { return GetNote() + L"\nAt: " + GetLocation(); }
      virtual std::wstring GetExceptionType() const override { return L"Windows Exception"; }
   };
public:
   MainWindow( HINSTANCE hInst, wchar_t* pArgs );
   MainWindow( const MainWindow& ) = delete;
   MainWindow& operator=( const MainWindow& ) = delete;
   // returns false if quitting
   void ProcessMessage();
private:
   static LRESULT WINAPI _HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
   static LRESULT WINAPI _HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
   LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
public:
   Keyboard kbd;
   Mouse mouse;
};