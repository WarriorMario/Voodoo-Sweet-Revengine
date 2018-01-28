#include "GUI.h"
#include "Graphsicks\Renderer.h"
#include "Gameplay\Player.h"

GUI::GUI()
{
	gameTime = new Timer();
	backgroundTime = new Texture("Images/Background.png");
	backGroundWaterPercentage = new Texture("Images/BackgroundWaterPercentage.png");
	fillingWaterPercentage = new Texture("Images/WaterPercentage2.png");
	frontWaterPercentage = new Texture("Images/BodyWaterPercentage.png");
}

GUI::~GUI()
{
	gameTime->Reset();
}

 void GUI::Update()
{
	 gameTimer();
	 waterPercentage = (Player::water_added_to_goal / Player::water_needed_to_win) * 100.0f;
	 if (waterPercentage == Player::water_needed_to_win) {
		 done = true;
	 }
	 else
		 if (displayTimeSeconds== 0 && displayTimeMinutes == 0) {
			 win = true;
		 }
}

 void GUI::Draw(Renderer& renderer,int xOffset, int yOffset)
{
	 String gameTimeText;
	 if (displayTimeSeconds < 10 && displayTimeMinutes < 10)
		 gameTimeText = "Time: 0" + std::to_string(displayTimeMinutes) + ":0" + std::to_string(displayTimeSeconds);
	 else if (displayTimeMinutes < 10)
		 gameTimeText = "Time: 0" + std::to_string(displayTimeMinutes) + ":" + std::to_string(displayTimeSeconds);
	 else if (displayTimeSeconds < 10)
		 gameTimeText = "Time: " + std::to_string(displayTimeMinutes) + ":0" + std::to_string(displayTimeSeconds);
	 else
		 gameTimeText = "Time: " + std::to_string(displayTimeMinutes) + ":" + std::to_string(displayTimeSeconds);
	 b2Vec2 pos = b2Vec2(200,200);
	 b2Vec2 wH = b2Vec2(waterPercentage*1.3f, 50);
	 RenderText(renderer, gameTimeText, 32, b2Vec2(Graphics::ScreenWidth / 2, Graphics::ScreenHeight - 50), Colors::White);
	 if (done) { RenderText(renderer, "The childs win", 32, b2Vec2(Graphics::ScreenWidth / 2, Graphics::ScreenHeight - 50), Colors::White); }
	 if (win)	{RenderText(renderer, " The neighbour wins", 32, b2Vec2(Graphics::ScreenWidth / 2, Graphics::ScreenHeight - 50), Colors::White);}
	 RenderQuad<ForegroundShader>(renderer, b2Vec2(1664, 1984), b2Vec2(180, 50), false, false, 0x00FFFFFF, backGroundWaterPercentage);
	 if (waterPercentage != 0)
	 {
		 RenderQuad<ForegroundShader>(renderer, b2Vec2(1664 + 25, 1984), wH, false, false, 0x00FFFFFF, fillingWaterPercentage);
	 }
	 RenderQuad<ForegroundShader>(renderer, b2Vec2(1664, 1984), b2Vec2(180, 50), false, false, 0x00FFFFFF, frontWaterPercentage);
}

void  GUI::gameTimer()
{
	time = 300;
	displayTimeSeconds = (int)gameTime->Time();
	displayTimeSeconds = time - displayTimeSeconds;
	displayTimeMinutes = displayTimeSeconds/60;
	displayTimeSeconds = displayTimeSeconds - displayTimeMinutes * 60;

}