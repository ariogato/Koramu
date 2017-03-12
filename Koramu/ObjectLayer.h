#pragma once
#include "Layer.h"
#include "Vector2D.h"
#include "Player.h"

class ObjectLayer : public Layer
{
private:
	Vector2D m_gameObjects;
	Player m_Player;

public:
	void update() {};
	void render() {};
};