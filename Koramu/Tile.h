#pragma once
#include "Vector2D.h"
#include <string>

class Tile
{
private:
	std::string m_tileTextureID;
	Vector2D m_positionVector;
	std::string m_message;
	int m_width;
	int m_height;

public:
	void load();
	void update();
	void render();

	std::string getTileTextureID() const { return m_tileTextureID; }
	std::string getMessage() const { return m_message; }
	int getHeight() const { return m_height; }
	int getWidth() const { return m_width; }
	Vector2D getPostionVector() const { return m_positionVector; }

	

};