#include "TileLayer.h"
#include <algorithm>
#include "TextureManager.h"

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
		}
	}
}
