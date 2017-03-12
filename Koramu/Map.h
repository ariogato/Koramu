#pragma once
#include "Vector2D.h"
#include <string>


class Map
{
private:
	Vector2D m_positionVector;
	std::string m_layers;
	
public:
	void render();
	void update();
};