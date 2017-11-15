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
#include "Vec3.h"
#include "Vec2.h"
#include "Physics\PhysicsConstants.h"

#include "Utility\ProfileOutput.h"

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
  arena(),
  player(renderer),
  input(godWindow.kbd,godWindow.mouse)
{
  test = arena.Create<PhysicsObject>();
  arena.physx.CreateDebugDraw(gfx);

  godWindow.SetFocused();

  test0.type = b2_dynamicBody; //this will be a dynamic body
  test0.position.Set(110, 20); //set the starting position
  test0.angle = 0; //set the starting angle

  b2CircleShape circle;
  circle.m_radius = 5;
  test1.type = b2_staticBody; //this will be a dynamic body
  test1.position.Set(100, 160); //set the starting position
  test1.angle = 0; //set the starting angle

  //for ( int i = 0; i < 5000; ++i )
  //{
  //   b = world.CreateBody( &test0 );
  //   testf0.shape = &circle;
  //   testf0.friction = 0.5f;
  //   testf0.density = 1.0f;
  //   b->SetTransform(Vec2(i,i%5),0);
  //   b->CreateFixture( &testf0 );
  //}
  //for(int i = 1; i < 80; ++i)
  //{
  //  temp = world.CreateBody(&test1);
  //  testf1.shape = &circle;
  //  testf1.friction = 0.5f;
  //  testf1.density = 1.0f;
  //  temp->SetTransform(Vec2(i * 10, 590), 0);
  //  temp->CreateFixture(&testf1);
  //  
  //  temp = world.CreateBody(&test1);
  //  testf1.shape = &circle;
  //  testf1.friction = 0.5f;
  //  testf1.density = 1.0f;
  //  temp->SetTransform(Vec2(i * 10, 0), 0);
  //  temp->CreateFixture(&testf1);
  //}
  //for(int i = 1; i < 80; ++i)
  //{
  //  temp = world.CreateBody(&test1);
  //  testf1.shape = &circle;
  //  testf1.friction = 0.5f;
  //  testf1.density = 1.0f;
  //  temp->SetTransform(Vec2(10, i * 10), 0);
  //  temp->CreateFixture(&testf1);
  //  temp = world.CreateBody(&test1);
  //  testf1.shape = &circle;
  //  testf1.friction = 0.5f;
  //  testf1.density = 1.0f;
  //  temp->SetTransform(Vec2(790, i * 10), 0);
  //  temp->CreateFixture(&testf1);
  //}

  //const b2ParticleSystemDef particleSystemDef;
  //particleSystem = world.CreateParticleSystem(&particleSystemDef);
  //particleSystem->SetRadius(3);
  //for ( int i = 0; i < 100000; ++i )
  //{
  //   b2ParticleDef pd;
  //   pd.flags = b2_waterParticle;
  //   pd.color.Set( 0, 0, 255, 255 );
  //   pd.position.Set( 20 +i*3, 80+i*5 );
  //   int tempIndex = particleSystem->CreateParticle( pd );
  //}

  
}

void Game::Go()
{
  ProfilerLogHandler pf_output;
  ProfileSample::output_handler = &pf_output;
  input.Poll();
  {
    PROFILE_SCOPE("Game::Go");

    gfx.BeginFrame();
    UpdateModel();
    ComposeFrame();
    gfx.EndFrame();
  }

  ProfileSample::Output();
}
Vec3 offset = Vec3(400, 400, 350);
void Game::UpdateModel()
{
  PROFILE_SCOPE("Game::UpdateModel");

  arena.Update();
  if(input.IsPressed(ButtonCode::ESC) == true)
  {
    godWindow.Kill();
  }
  
  player.Update();
  player.Input(godWindow.kbd);
  font_render_object.Update();

  static size_t count = 0;
  if(input.IsPressed(ButtonCode::GAMEPAD_A,1) == true )
  {
    count++;
    if(count % 20)
    {
      auto particle = arena.Create<ParticleObject>();
      particle->SetPosition(input.MousePos());
    }
  }
  test->SetPosition(Vec2(input.MousePos().x + 10 * input.GetAxis(AxisCode::LEFT,0).x, input.MousePos().y + 10 * input.GetAxis(AxisCode::LEFT, 0).y));
}

void Game::ComposeFrame()
{
  PROFILE_SCOPE("Game::ComposeFrame");

  arena.Draw(renderer);
  player.Draw();
  font_render_object.Draw(renderer);
  renderer.Render();
  arena.physx.DebugDraw();
}