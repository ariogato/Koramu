#include "TileLayer.h"
#include <algorithm>
#include "TextureManager.h"
#include "Game.h"
#include "ObjectRectangle.h"
#include "ParamLoader.h"

Environment::TileLayer::TileLayer()
{
}

Environment::TileLayer::~TileLayer()
{
	//	Wir löschen iterativ die Tiles im zweidimensionalen Vector aus Tiles.
	for(auto i : m_tiles)
	{
		for(auto j : i)
		{
			delete j;
		}
	}
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
	//	Die folgenden for-Schleifen iterieren über alle Tiles, damit jedes einzeln gerendert werden kann.
	for (int i = 0; i < m_tiles.size(); i++)
	{
		for (int j = 0; j < m_tiles[i].size(); j++)
		{
			/*	Hier wird erschlossen, zu welchem Tileset das aktuelle Tile gehört.
			 *
			 *	'std::find_if()' gibt gibt einen 'iterator' auf das gewünschte Tileset zurück.
			 */
			int tileId = m_tiles[i][j]->getTileID();
			std::vector<Environment::Tileset>::iterator tilesetToUse = std::find_if(m_tilesets.begin(), m_tilesets.end(), [tileId](const Tileset& t)
			{
				return tileId < t.firstgid + t.tilecount;
			});

			
			//	Zuletzt wird das Tile mit Hilfe des 'TextureManager's gerenderts.
			TheTextureManager::Instance()->drawTile(*tilesetToUse, m_tiles[i][j]->getTileID(), j * 64 + m_tiles[i][j]->getPostionVector().getX(), i * 64 + m_tiles[i][j]->getPostionVector().getY());

			if(tilesetToUse->collisionMap.count(tileId - tilesetToUse->firstgid))
			{
				for (int k = 0; k < tilesetToUse->collisionMap[tileId - tilesetToUse->firstgid].size(); k++)
				{
					Collisionbox cBox = tilesetToUse->collisionMap[tileId - tilesetToUse->firstgid][k];
					/*
					ObjectRectangle cRect;
					ParamLoader params;
					params.setX(j * 64 + m_tiles[i][j]->getPostionVector().getX() + cBox.xPos);
					params.setY(i * 64 + m_tiles[i][j]->getPostionVector().getY() + cBox.yPos);
					params.setWidth(cBox.width);
					params.setHeight(cBox.height);
					cRect.load(params);
					cRect.setVisible(true);
					cRect.setShowText(true);
					cRect.draw();
					*/
					
					SDL_Color cRectColor;
					cRectColor.r = 0; cRectColor.g = 0, cRectColor.b = 255, cRectColor.a = 255;
					SDL_Rect rect = { j * 64 + m_tiles[i][j]->getPostionVector().getX() + cBox.xPos, i * 64 + m_tiles[i][j]->getPostionVector().getY() + cBox.yPos, cBox.width, cBox.height };
					SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), cRectColor.r, cRectColor.g, cRectColor.b, cRectColor.a);
					SDL_RenderDrawRect(TheGame::Instance()->getRenderer(), &rect);
					SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 0, 0, 255);
					
				}				
			}
			
			
		}
	}
}

int Environment::TileLayer::getTileIdAtPosition(const Vector2D& positionVector) const 
{
	//	Überprüfen, ob der 'positionVector' die Grenzen der Matrix nicht überschreitet
	if (positionVector.getY() / 64 < 0 || positionVector.getY() / 64 > m_tiles.size() ||
		positionVector.getX() / 64 < 0 || positionVector.getX() / 64 > (*m_tiles.begin()).size() - 1)
		return 0;

	//	TileId des Tiles an der gewünschten Position ermitteln und zurückgeben
	return m_tiles[positionVector.getY() / 64][positionVector.getX() / 64]->getTileID();
}
