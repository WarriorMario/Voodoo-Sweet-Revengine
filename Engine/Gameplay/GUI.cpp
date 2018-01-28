#include "GUI.h"

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
	 waterPercentage += 1;
}

 void GUI::Draw(Renderer& renderer)
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
	 b2Vec2 pos = b2Vec2(Graphics::ScreenWidth / 2,Graphics::ScreenHeight/2);
	 b2Vec2 wH = b2Vec2(waterPercentage*1.3f, 50);
	 RenderText(renderer, gameTimeText,32, b2Vec2(pos.x,pos.y), Colors::White);
	 RenderQuad<ForegroundShader>(renderer, b2Vec2(pos.x,pos.y), b2Vec2(180, 50), false,false, 0x00FFFFFF, backgroundTime);
	 RenderQuad<ForegroundShader>(renderer, b2Vec2(pos.x, pos.y), b2Vec2(180, 50), false, false, 0x00FFFFFF, backGroundWaterPercentage);
	 if (waterPercentage != 0)
	 {
		 RenderQuad<ForegroundShader>(renderer, b2Vec2(pos.x + 25, pos.y), wH, false, false, 0x00FFFFFF, fillingWaterPercentage);
	 }
	 RenderQuad<ForegroundShader>(renderer, b2Vec2(pos.x, pos.y), b2Vec2(180, 50), false, false, 0x00FFFFFF, frontWaterPercentage);
}

void  GUI::gameTimer()
{
	displayTimeSeconds = (int)gameTime->Time();
	displayTimeMinutes = displayTimeSeconds/60;
	displayTimeSeconds = displayTimeSeconds - displayTimeMinutes * 60;
}