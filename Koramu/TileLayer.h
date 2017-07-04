#pragma once

#include <vector>
#include "Layer.h"
#include "Tile.h"
#include "Map.h"

namespace Environment
{

	class TileLayer : public Layer
	{
	private:
		std::vector<Tileset> m_tilesets;					//	Vector aus Tileset Objekten, die zum Rendern einzelner Tiles wichtig sind
		std::vector<std::vector<Tile*>> m_tiles;			//	Mehrdimensionaler Vector aus zu dem Layer gehörigen Tiles

	public:
		TileLayer();
		~TileLayer();

		void init(std::vector<Tileset> tilesets, std::vector<std::vector<Tile*>> tiles);				//	Initialisieren
		void update() override;																			//	Aktualisieren
		void render() override;																			//	Rendern

		//	getter-Funktionen
		int getTileIdAtPosition (const Vector2D& positionVector) const;
		std::vector<Tileset> getTilesets() const { return m_tilesets; };
	};
}

