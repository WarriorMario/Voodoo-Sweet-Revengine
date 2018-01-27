/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Game.h																				  *
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
#pragma once

#include "LiquidFun/Box2D.h"

#include "MainWindow.h"
#include "Graphsicks\Renderer.h"
#include "Graphsicks/Graphics.h"

#include "Input\Input.h"

#include "Gameplay\Framework\Objects\FrameCounter.h"
#include "Gameplay\Scene.h"

class Game
{
public:
  Game(HINSTANCE hInst, LPWSTR pArgs);
  ~Game();

  void Go();
  
  virtual void OnInitialize() = 0;
  virtual void OnTerminate() = 0;
  virtual void OnUpdate(float dt) = 0;
  virtual void OnRender() = 0;

private:
  void Update();
  void Render();

  void EndFrame();
  void BeginFrame();

protected:
  // Rendering
  Renderer god_view;
  Renderer player_view;
  Input* input;

private:
  // Displaying
  MainWindow godWindow;
  RenderWindow playerWindow;
  Graphics* gfx;

  // Updating
  Timer timer;
  bool show_frame_counter = false;
  FrameCounter frame_counter;

  // IDK what this stuff is doing here
  b2ParticleSystem* particleSystem;
  //GodEditor editor;

};