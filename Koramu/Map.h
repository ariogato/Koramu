#pragma once
#include "Vector2D.h"
#include <map>
#include <vector>
#include "Layer.h"

//class Layer;
class GameObject;

//	Alles bezüglich der Map befindet sich im Namenraum "Environment"
namespace Environment
{
	//	Instanzen dieser Klasse repräsentieren unsere Maps.
	class Map
	{
	private:
		Vector2D m_positionVector;						//	Ortsvektor
		std::map<std::string, Layer*> m_layerMap;		//	Dictionary/Map aus zur Map gehörigen Layern

	public:
		Map();
		~Map();

		void init(std::vector<GameObject*>* pObjects);			//	Initialisieren
		void update();									//	Aktualisieren
		void render();									//	Rendern
	};
}
