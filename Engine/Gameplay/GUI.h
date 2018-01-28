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
	int time = 300;
	int displayTimeMinutes = 0;
	int displayTimeSeconds = 0;
	bool done = false;
	bool win = false;
	Texture* backgroundTime;
	Texture* backGroundWaterPercentage;
	Texture* fillingWaterPercentage;
	Texture* frontWaterPercentage;
	float waterPercentage;
};



