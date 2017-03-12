#pragma once
#include "Layer.h"
#include "Vector2D.h"

class TileLayer : public Layer
{
private:
	Vector2D m_tiles;

public:
	void update() {};
	void render() {};
};
