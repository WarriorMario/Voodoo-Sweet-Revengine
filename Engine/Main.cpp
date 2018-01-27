/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Main.cpp																			  *
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

#include "Assets/Assets.h"
#include "Gameplay/Game.h"

#include "ChiliException.h"

#include "Utility/Logging.h"
#include "Utility/ResourceAllocator.h"

#include <random>
#include <time.h>

//*****************************************************************************

class OurGame : public Game
{
public:
  OurGame(HINSTANCE hInst, LPWSTR pArgs)
    : Game(hInst, pArgs)
  {
  }

  void OnInitialize() override
  {
    scene.Init();

    scene.SpawnPlayer(0);
    scene.SpawnPlayer(1);
    scene.SpawnPlayer(2);
    scene.NewGod();
  }
  void OnTerminate() override
  {
  }
  void OnUpdate(float dt) override
  {
    scene.Tick(dt, *input);

    // Adjust the camera's for both player groups
    god_view.AdjustCamera({(Player*)scene.GetGod()});
    player_view.AdjustCamera(scene.GetPlayers());
  }
  void OnRender() override
  {
    scene.Draw();
  }

private:
  Scene scene;

};

//*****************************************************************************

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	std::srand(time(NULL));
  try
  {
    Logger::Get().Init();

    MakeSingleton<Assets>();

    MakeSingleton<ResourceAllocator>();
    ResourceAllocator::Get().Allocate<int>(1024 * 1024 * 40);

    { // Extra scope here so game releases its resources before assets get freed
      OurGame theGame(hInst, pArgs);
      theGame.Go();
    }

    DestroySingleton<ResourceAllocator>();
    
    DestroySingleton<Assets>();
  }
  catch(const ChiliException& e)
  {
    const std::wstring eMsg = e.GetFullMessage() +
      L"\n\nException caught at main window creation.";
    MessageBox(nullptr, eMsg.c_str(), e.GetExceptionType().c_str(), MB_OK);
  }
  catch(const std::exception& e)
  {
    // need to convert std::exception what() string from narrow to wide string
    const String whatStr(e.what());
    const std::wstring eMsg = std::wstring(whatStr.begin(), whatStr.end()) +
      L"\n\nException caught at main window creation.";
    MessageBox(nullptr, eMsg.c_str(), L"Unhandled STL Exception", MB_OK);
  }
  catch(...)
  {
    MessageBox(nullptr, L"\n\nException caught at main window creation.",
      L"Unhandled Non-STL Exception", MB_OK);
  }

  return 0;
}