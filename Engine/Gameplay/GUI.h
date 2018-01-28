#pragma once
#include "Framework\States.h"
#include "Matrix.h"
#include "Utility\Timer.h"
#include "Utility\DrawUtils.h"
#include <iostream>
class GUI
{
public:
	GUI();
	~GUI();
	void Update();
	void Draw(Renderer & renderer, int x, int y);
	void gameTimer();
	Timer* gameTime;
	int x;
	int y;
private:
	int displayTimeMinutes = 0;
	int displayTimeSeconds = 0;
	bool done;
	Texture* backgroundTime;
	Texture* backGroundWaterPercentage;
	Texture* fillingWaterPercentage;
	Texture* frontWaterPercentage;
	float waterPercentage;
};



