#include "Tile.h"
#include "ParamLoader.h"

Environment::Tile::Tile()
	: m_tileID(0), m_positionVector(0.0f, 0.0f), m_width(64), m_height(64)
{
}

Environment::Tile::~Tile()
{
}

void Environment::Tile::load(const ParamLoader& params)
{
	m_width = params.getWidth();
	m_height = params.getHeight();
	m_tileID = params.getTileId();
	m_positionVector.setX(params.getX());
	m_positionVector.setY(params.getY());
}

void Environment::Tile::update()
{

}

void Environment::Tile::draw()
{
	
}
