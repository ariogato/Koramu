#include "TileLayer.h"

Environment::TileLayer::TileLayer()
{
}

Environment::TileLayer::~TileLayer()
{
}

void Environment::TileLayer::init(std::vector<Tileset> tilesets, std::vector<std::vector<Tile*>> tiles)
{
	m_tiles = tiles;
	m_tilesets = tilesets;
}

void Environment::TileLayer::update()
{
}

void Environment::TileLayer::render()
{
}
