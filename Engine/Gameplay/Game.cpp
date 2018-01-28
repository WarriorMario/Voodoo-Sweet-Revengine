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
#include "Matrix.h"
#include "Gameplay\Scene.h"

#include "Physics\PhysicsConstants.h"
#include "Utility\FrameAllocator.h"
#include "Threading\Job_Manager.h"

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

Game::Game(HINSTANCE hInst, LPWSTR pArgs)
  :
  godWindow(hInst, pArgs),
  playerWindow(hInst, pArgs)
{
  gfx = new Graphics(godWindow, playerWindow);
  input = new Input(godWindow.kbd, godWindow.mouse);
  //gui = new GUI();
  JM_Get() = JM_AllocJobManager();
  JM_InitJobManager(JM_Get(), num_cores);
  
  MakeSingleton<FrameAllocator>();

  godWindow.SetFocused();
}

Game::~Game()
{
  delete input;
  delete gfx;

  DestroySingleton<FrameAllocator>();
}

void Game::Go()
{
  ProfilerLogHandler pf_output;
  ProfileSample::output_handler = &pf_output;

  OnInitialize();

  while(RenderWindow::IsOpen())
  {
    Update();
    Render();

    // clear the frame allocator.
    FrameAllocator::Get().Clear();

    // I/O tasks
    godWindow.ProcessMessage();
    input->Poll();

    ProfileSample::Output();
  }

  OnTerminate();
}
void Game::Update()
{
  PROFILE_SCOPE("Game::UpdateModel");

  // Some hard-coded input stuff
  if(input->IsPressed(ButtonCode::ESC) == true)
  {
    godWindow.Kill();
  }
  if(input->IsPressed(ButtonCode::TWO) == true)
  {
    show_frame_counter = !show_frame_counter;
  }
  if (input->IsPressed(ButtonCode::SPACE) == true) 
  {
	  gui.Update();
  }
  OnUpdate(timer.Elapsed()); // Delegate call to concrete implementation

  timer.Reset();
}

void Game::Render()
{
  PROFILE_SCOPE("Game::Render");

  BeginFrame();

  OnRender(); // Delegate call to concrete implementation
  gui.Draw(god_view,god_view.camera.offset.x, god_view.camera.offset.y);
  gui.Draw(player_view, player_view.camera.offset.x, player_view.camera.offset.y);
  // Frame-counter is only displayed on the primary window for now
  if(show_frame_counter)
  {
    frame_counter.Draw(god_view);
  }

  player_view.Render(gfx->player_window_buffer);
  god_view.Render(gfx->god_window_buffer);
  
  OnLateRender();

  EndFrame();
}

void Game::BeginFrame()
{
  PROFILE_SCOPE("Game::Go::BeginFrame");
  gfx->BeginFrame();
}
void Game::EndFrame()
{
  PROFILE_SCOPE("Game::Go::EndFrame");
  gfx->EndFrame();

  god_view.Reset();
  player_view.Reset();
}