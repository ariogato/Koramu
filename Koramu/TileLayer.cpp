#include "TileLayer.h"
#include <algorithm>
#include "TextureManager.h"
#include "Game.h"
#include "ObjectRectangle.h"
#include "ParamLoader.h"
#include "Camera.h"

Environment::TileLayer::TileLayer()
{
}

Environment::TileLayer::~TileLayer()
{
	//	Wir l�schen iterativ die Tiles im zweidimensionalen Vector aus Tiles.
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
	/*	Im Folgenden wird f�r jedes Tile des "TileLayer"s �berpr�ft, ob es sich (zumindest teilweise)
	*	innerhalb des durch die Kamera festgelegten Rechtecks befindet.
	*	Nur Tiles, f�r die das der Fall ist sollen gezeichnet werden. Die anderen sind f�r den Benutzer sowieso nicht sichtbar.
	*
	*	Diese �berpr�fung �hnelt stark der Kollisionserkennung. Betrachtet werden hier die Tiles und das, durch die
	*	Kamera festgelegte Rechteck. F�r weitere Informationen siehe "ObjectLayer.cpp" oder unser Wiki.
	*/

	//	Definition von Variablen zur Speicherung der Extremwerte der zu vergleichenden Rechtecke (axis-aligned - entlang der Achsen ausgerichtet)
	int topA, topB;						//	y-Wert von Punkten auf der oberen Kante
	int bottomA, bottomB;				//	y-Wert von Punkten auf der unteren Kante
	int leftA, leftB;					//	x-Wert von Punkten auf der linken Kante
	int rightA, rightB;					//	x-Wert von Punkten auf der rechten Kante

	//	Extremwerte des durch die Kamera festgelegten Rechtecks
	topA = TheGame::Instance()->getCamera()->getPositionVector().getY();
	bottomA = topA + TheGame::Instance()->getCamera()->getCameraHeight();
	leftA = TheGame::Instance()->getCamera()->getPositionVector().getX();
	rightA = leftA + TheGame::Instance()->getCamera()->getCameraWidth();

	//	Die folgenden for-Schleifen iterieren �ber alle Tiles, damit jedes, das obige Bedingung erf�llt, gerendert werden kann.
	for (int i = 0; i < m_tiles.size(); i++)
	{
		for (int j = 0; j < m_tiles[i].size(); j++)
		{
			/*	Hier wird erschlossen, zu welchem Tileset das aktuelle Tile geh�rt.
			 *
			 *	'std::find_if()' gibt gibt einen 'iterator' auf das gew�nschte Tileset zur�ck.
			 */
			int tileId = m_tiles[i][j]->getTileID();
			std::vector<Environment::Tileset>::iterator tilesetToUse = std::find_if(m_tilesets.begin(), m_tilesets.end(), [tileId](const Tileset& t)
			{
				return tileId < t.firstgid + t.tilecount;
			});

			//	Extremwerte des aktuellen Tiles
			topB = i * 64 + m_tiles[i][j]->getPostionVector().getY();
			bottomB = topB + 64;
			leftB = j * 64 + m_tiles[i][j]->getPostionVector().getX();
			rightB = leftB + 64;

			//	Wenn folgende Bedingungen alle zutrefen, dann ber�hren oder �berlappen sich die Rechtecke - das Tile soll gerendert werden
			if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
			{
				/*	Zuletzt wird das Tile mit Hilfe des 'TextureManager's gerendert.
				 *	Die Posiition ist dabei abh�ngig von der Position des Layers, welche abh�ngig von der Position der Kamera ist
				 *	und in "Map::update()" f�r jeden Frame aktualisiert wird.
				 */
				TheTextureManager::Instance()->drawTile(*tilesetToUse, m_tiles[i][j]->getTileID(), j * 64 + m_tiles[i][j]->getPostionVector().getX() + m_positionVector.getX(), i * 64 + m_tiles[i][j]->getPostionVector().getY() + m_positionVector.getY());
				
				//	Folgender Code zeichnet die Kollisionsrechtecke des Tiles, falls es welche hat. Dies ist nur zum Debuggen wichtig, bedarf also keiner ausf�hrlichen Erkl�rung.
				if (tilesetToUse->collisionMap.count(tileId - tilesetToUse->firstgid))
				{
					for (int k = 0; k < tilesetToUse->collisionMap[tileId - tilesetToUse->firstgid].size(); k++)
					{
						Collisionbox cBox = tilesetToUse->collisionMap[tileId - tilesetToUse->firstgid][k];
						
						ObjectRectangle cRect;
						ParamLoader params;
						params.setX(j * 64 + m_tiles[i][j]->getPostionVector().getX() + cBox.xPos);
						params.setY(i * 64 + m_tiles[i][j]->getPostionVector().getY() + cBox.yPos);
						params.setWidth(cBox.width);
						params.setHeight(cBox.height);
						cRect.load(params);
						cRect.setVisible(true);
						cRect.setShowText(false);
						cRect.setColor(0, 0, 255, 255);
						cRect.draw(m_positionVector);
					}
				}
			}
		}
	}
}

int Environment::TileLayer::getTileIdAtPosition(const Vector2D& positionVector) const 
{
	//	�berpr�fen, ob der 'positionVector' die Grenzen der Matrix nicht �berschreitet
	if (positionVector.getY() / 64 < 0 || positionVector.getY() / 64 > m_tiles.size() ||
		positionVector.getX() / 64 < 0 || positionVector.getX() / 64 > (*m_tiles.begin()).size() )
		return 0;

	//	TileId des Tiles an der gew�nschten Position ermitteln und zur�ckgeben
	return m_tiles[positionVector.getY() / 64][positionVector.getX() / 64]->getTileID();
}
