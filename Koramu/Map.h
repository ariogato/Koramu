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
	class TileLayer;

	/*	Eine Datenstruktur zur Speicherung der essentiellen Daten von Kollisionsboxen
	 */
	struct Collisionbox
	{
		std::string id;
		float xPos;
		float yPos;
		float width;
		float height;

		Collisionbox()
			: xPos(0), yPos(0), width(0), height(0)
		{}
	};

	/*	Eine Datenstruktur zur Speicherung der essentiellen Daten von Tilesets
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
		std::map<int, std::vector<Collisionbox>> collisionMap;

		Tileset()
			: firstgid(0), tilewidth(0), tileheight(0), tilecount(0), numCols(0), numRows(0)
		{}
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

		//	getter-Funktionen
		TileLayer* getCollisionLayer();
	};
}
