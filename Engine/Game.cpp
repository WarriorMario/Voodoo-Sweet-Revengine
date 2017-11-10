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
#include "Utility\Timer.h"
b2Body* b;
b2Body* temp;
b2BodyDef test0;
b2BodyDef test1;
b2FixtureDef testf0;
b2FixtureDef testf1;
Game::Game(MainWindow& godWindow, RenderWindow& playerWindow)
  :
  godWindow(godWindow),
  playerWindow(playerWindow),
  gfx(godWindow, playerWindow),
  world(b2Vec2(0, 9.81f)),
  renderer(Renderer(gfx)),
  arena()
{
  auto test = arena.Create<GameObject>();

  godWindow.SetFocused();
  Vec3* verts = new Vec3[6];
  verts[0] = Vec3(50, 100, 1);
  verts[1] = Vec3(100, 52, 1);
  verts[2] = Vec3(10, 55, 1);
  verts[3] = Vec3(50, 100, 1);
  verts[4] = Vec3(100, 52, 1);
  verts[5] = Vec3(10, 55, 100);

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
  //   b->SetTransform(b2Vec2(i,i%5),0);
  //   b->CreateFixture( &testf0 );
  //}
  for(int i = 1; i < 80; ++i)
  {
    temp = world.CreateBody(&test1);
    testf1.shape = &circle;
    testf1.friction = 0.5f;
    testf1.density = 1.0f;
    temp->SetTransform(b2Vec2(i * 10, 590), 0);
    temp->CreateFixture(&testf1);
    temp = world.CreateBody(&test1);
    testf1.shape = &circle;
    testf1.friction = 0.5f;
    testf1.density = 1.0f;
    temp->SetTransform(b2Vec2(i * 10, 0), 0);
    temp->CreateFixture(&testf1);
  }
  for(int i = 1; i < 80; ++i)
  {
    temp = world.CreateBody(&test1);
    testf1.shape = &circle;
    testf1.friction = 0.5f;
    testf1.density = 1.0f;
    temp->SetTransform(b2Vec2(10, i * 10), 0);
    temp->CreateFixture(&testf1);
    temp = world.CreateBody(&test1);
    testf1.shape = &circle;
    testf1.friction = 0.5f;
    testf1.density = 1.0f;
    temp->SetTransform(b2Vec2(790, i * 10), 0);
    temp->CreateFixture(&testf1);
  }
  const b2ParticleSystemDef particleSystemDef;
  particleSystem = world.CreateParticleSystem(&particleSystemDef);
  particleSystem->SetRadius(3);
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
  Timer frame_time;
  gfx.BeginFrame();
  UpdateModel();
  ComposeFrame();
  gfx.EndFrame();
  char buff[128];
  sprintf(buff, "%f \n", frame_time.Elapsed());
  OutputDebugStringA(buff);
}
Vec3 offset = Vec3(400, 400, 350);
void Game::UpdateModel()
{
  if(godWindow.kbd.KeyIsPressed(VK_ESCAPE) == true)
  {
    godWindow.Kill();
  }
  if(godWindow.mouse.LeftIsPressed() == true)
  {
	  tri_buff[0] = Triangle(
		  Vec2(300, 300),
		  Vec2(300, 200),
		  Vec2(400, 200),
		  Vec2(0.0f, 0.0f),
		  Vec2(0.0f, 0.0f),
		  Vec2(0.0f, 0.0f));
	  tri_buff[1] = Triangle(
		  Vec2(350, 300),
		  Vec2(400, 200),
		  Vec2(400, 300),
		  Vec2(0.0f, 0.0f),
		  Vec2(0.0f, 0.0f),
		  Vec2(0.0f, 0.0f));

	  BackgroundShader testASDASD;
	  testASDASD.const_data.color = Colors::Yellow;
    testASDASD.prim_data[0] = {300, 300, 0.0, 0.0f};
    testASDASD.prim_data[1] = {300, 200, 1.0f, 0.0f};
    testASDASD.prim_data[2] = {400, 200, 0.5f, 1.0f};

    ForegroundShader fg_tri;
    fg_tri.const_data.color = Colors::Red;
    fg_tri.prim_data[0] = {-50 + (float)godWindow.mouse.GetPosX() , (float)godWindow.mouse.GetPosY()+ 50, 0.0, 0.0f};
    fg_tri.prim_data[1] = {-50 + (float)godWindow.mouse.GetPosX(),(float)godWindow.mouse.GetPosY() - 50, 1.0f, 0.0f};
    fg_tri.prim_data[2] = {50 + (float)godWindow.mouse.GetPosX(), (float)godWindow.mouse.GetPosY() - 50, 0.5f, 1.0f};
	  //BackgroundShader asdsa;
	  //asdsa.const_data.color = Colors::Blue;
	  //asdsa.prim_data[0] = tri_buff[1].p[0];
	  //asdsa.prim_data[1] = tri_buff[1].p[1];
	  //asdsa.prim_data[2] = tri_buff[1].p[2];


	  renderer.AddDrawCommand(testASDASD);
	  renderer.AddDrawCommand(fg_tri);
    b2ParticleDef pd;
    pd.flags = b2_waterParticle;
    pd.color.Set(0, 0, 255, 255);
    pd.position.Set(godWindow.mouse.GetPosX(), godWindow.mouse.GetPosY());
    int tempIndex = particleSystem->CreateParticle(pd);
  }
  if(godWindow.mouse.RightIsPressed() == true)
  {
    char buff[125];
    sprintf(buff, "%d\n", particleSystem->GetParticleCount());
    OutputDebugStringA(buff);
  }
  world.Step(1.0f / 60.0f, 1, 1);
}

void Game::ComposeFrame()
{
  for(b2Body* current = world.GetBodyList(); current != nullptr; current = current->GetNext())
  {
    gfx.DrawClippedLineCircle(5, *(Vec2*)&current->GetPosition(), 12, RectF(0, Graphics::ScreenHeight - 1, 0, Graphics::ScreenWidth - 1));
  }
  for(int i = 0; i < particleSystem->GetParticleCount(); ++i)
  {
    gfx.DrawClippedLineCircle(3, *(Vec2*)&particleSystem->GetPositionBuffer()[i], 12, RectF(0, Graphics::ScreenHeight - 1, 0, Graphics::ScreenWidth - 1));
  }
  renderer.Render();
//  Rasterizer rasterizer;
  //Shader shader(gfx);
  //rasterizer.RasterizeCells(renderer.grid.cells, tri_buff, shader,
  //  renderer.grid.CELL_WIDTH, renderer.grid.CELL_HEIGHT,
  //  renderer.grid.WIDTH, renderer.grid.HEIGHT);
}