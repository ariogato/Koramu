#pragma once
#include "Vector2D.h"
#include "Layer.h"
#include "map"

//	Alles bezüglich der Map befindet sich im Namenraum "Environment"
namespace Environment
{
	//	Instanzen dieser Klasse repräsentieren unsere Maps.
	class Map
	{
	private:
		Vector2D m_positionVector;					//	Ortsvektor
		std::map<std::string, Layer*> m_layerMap;		//	Dictionary/Map aus zur Map gehörigen Layern

	public:
		Map();
		~Map();

		void update();			//	Aktualisieren
		void render();			//	Rendern
	};
}
