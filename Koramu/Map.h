#pragma once
#include "Vector2D.h"
#include "Layer.h"
#include "map"

//	Alles bez�glich der Map befindet sich im Namenraum "Environment"
namespace Environment
{
	//	Instanzen dieser Klasse repr�sentieren unsere Maps.
	class Map
	{
	private:
		Vector2D m_positionVector;					//	Ortsvektor
		std::map<std::string, Layer*> m_layerMap;		//	Dictionary/Map aus zur Map geh�rigen Layern

	public:
		Map();
		~Map();

		void update();			//	Aktualisieren
		void render();			//	Rendern
	};
}
