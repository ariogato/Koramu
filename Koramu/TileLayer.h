#pragma once
#include "Layer.h"
#include <vector>
#include "Tile.h"

namespace Environment
{
	class TileLayer : public Layer
	{
	private:
		std::vector<std::vector<Tile*>> m_tiles;		//	Mehrdimensionaler Vector aus zu dem Layer gehörigen Tiles

	public:
		TileLayer();
		~TileLayer();

		void update();				//	Aktualisieren
		void render();				//	Rendern
	};
}

