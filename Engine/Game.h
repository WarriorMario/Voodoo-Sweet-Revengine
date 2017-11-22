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

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphsicks/Graphics.h"
#include "LiquidFun/Box2D.h"
#include "Graphsicks\Renderer.h"
#include "Triangle.h"
#include "Gameplay\Framework\Arena.h"
#include "Assets\Assets.h"
#include "Gameplay\Player.h"
#include "Input\Input.h"
#include "Gameplay\Framework\Objects\FontRenderObject.h"
#include "Gameplay\Framework\Objects\FrameCounter.h"
#include "Threading\Job_Manager.h"
#include "Gameplay\GodEditor.h"
class Game
{
public:
	Game(class MainWindow& wnd, class RenderWindow& wnd2);
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	~Game();
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& godWindow;
	RenderWindow& playerWindow;
	Graphics gfx;
	b2ParticleSystem* particleSystem;
	Renderer renderer;
	Triangle tri_buff[2];
  FrameCounter frame_counter;
  Arena arena;
  GodEditor editor;
  Player player;
  Input input;
	/********************************/
	/*  User Variables              */
	/********************************/
};