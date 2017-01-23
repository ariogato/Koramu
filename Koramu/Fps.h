#pragma once
#include <iostream>
#include <SDL.h>

class Fps
{
private: 
	float fps;
	float frameTime;

public:
	void calculateFPS();
};