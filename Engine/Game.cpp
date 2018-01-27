/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Game.cpp																			  *
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
#include "Game.h"
#include "MainWindow.h"
#include "Matrix.h"
#include "Physics\PhysicsConstants.h"
#include "Utility\FrameAllocator.h"
#include "Gameplay\Scene.h"

#include "Utility\ProfileOutput.h"
#ifdef NDEBUG
#if SINGLE_CORE_PROFILING
constexpr size_t num_cores = 1;
#else

constexpr size_t num_cores = 8;
#endif
#else
constexpr size_t num_cores = 1;
#endif
b2Body* b;
b2Body* temp;
b2BodyDef test0;
b2BodyDef test1;
b2FixtureDef testf0;
b2FixtureDef testf1;
Handle<PhysicsObject> test;


Game::Game(MainWindow& godWindow, RenderWindow& playerWindow)
	:
	godWindow(godWindow),
	playerWindow(playerWindow),
	gfx(godWindow, playerWindow),
	renderer(Renderer(gfx)),
	arena(input),
	input(godWindow.kbd, godWindow.mouse),
	editor(arena),
	frame_counter(scene)
{
	JM_Get() = JM_AllocJobManager();
	JM_InitJobManager(JM_Get(), num_cores);
	//test = arena.Create<PhysicsObject>();
	arena.physx.CreateDebugDraw(gfx);
	editor.Init();

	MakeSingleton<FrameAllocator>();

	godWindow.SetFocused();

	test0.type = b2_dynamicBody; //this will be a dynamic body
	test0.position.Set(110, 20); //set the starting position
	test0.angle = 0; //set the starting angle

	b2CircleShape circle;
	circle.m_radius = 5;
	test1.type = b2_staticBody; //this will be a dynamic body
	test1.position.Set(100, 160); //set the starting position
	test1.angle = 0; //set the starting angle

	scene.Init();
}

Game::~Game()
{
	DestroySingleton<FrameAllocator>();
}

void Game::Go()
{
	ProfilerLogHandler pf_output;
	ProfileSample::output_handler = &pf_output;
	{
		PROFILE_SCOPE("Game::Go");
		{
			PROFILE_SCOPE("Game::Go::BeginFrame");
			gfx.BeginFrame();
		}
		UpdateModel();
		ComposeFrame();
		{
			PROFILE_SCOPE("Game::Go::EndFrame");
			gfx.EndFrame();
		}
	}

	// clear the frame allocator.
	FrameAllocator::Get().Clear();

	ProfileSample::Output();


	input.Poll();
}
Vec3 offset = Vec3(400, 400, 350);
void Game::UpdateModel()
{
	PROFILE_SCOPE("Game::UpdateModel");

	arena.Update();
	if (input.IsPressed(ButtonCode::ESC) == true)
	{
		godWindow.Kill();
	}
	editor.Update();
	frame_counter.Update();

	// tick the scene
	scene.Tick(godWindow.kbd);

	//test->SetPosition(Vec2(input.MousePos().x + 10 * input.GetAxis(AxisCode::LEFT,0).x, input.MousePos().y + 10 * input.GetAxis(AxisCode::LEFT, 0).y));
}

void Game::ComposeFrame()
{
	PROFILE_SCOPE("Game::ComposeFrame");
	//arena.Draw(renderer);
	scene.Draw(renderer);
	frame_counter.Draw(renderer);
	renderer.Render();
	arena.physx.DebugDraw();
}
