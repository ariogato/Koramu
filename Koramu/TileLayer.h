#pragma once
#include "Layer.h"
#include <vector>
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

		void init(std::vector<Tileset> tilesets, std::vector<std::vector<Tile*>> tiles);		//	Initialisieren
		void update();																			//	Aktualisieren
		void render();																			//	Rendern
	};
}

