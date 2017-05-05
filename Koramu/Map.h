#pragma once
#include "Vector2D.h"
#include <map>
#include <vector>
#include "Layer.h"

class GameObject;
class ParamLoader;

//	Alles bezüglich der Map befindet sich im Namenraum "Environment"
namespace Environment
{
	/*	Eine Datenstruktur zur Speicherung der essentiellen Daten von Tilesets
	 *
	 *	
	 */
	struct Tileset
	{
		int firstgid;
		int tilewidth;
		int tileheight;
		int tilecount;
		int numCols;
		int numRows;
		std::string name;
	};

	//	Instanzen dieser Klasse repräsentieren unsere Maps.
	class Map
	{
	private:
		int m_width;										//	Breite der Map in Tiles
		int m_height;										//	Höhe der Map in Tiles
		int m_tilewidth;									//	Breite der Tiles in Pixel
		int m_tileheight;									//	Höhe der Tiles in Pixel
		Vector2D m_positionVector;							//	Ortsvektor
		std::map<std::string, Layer*> m_layerMap;			//	Dictionary/Map aus zur Map gehörigen Layern

	public:
		Map();
		~Map();

		void init(	const ParamLoader& params,
					int tilewidth, int tileheight);			//	Initialisieren
		void update();										//	Aktualisieren
		void render();										//	Rendern

		void addLayer(std::string name, Layer* pNewLayer);
	};
}
