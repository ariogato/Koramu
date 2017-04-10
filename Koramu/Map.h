#pragma once
#include "Vector2D.h"
#include <map>
#include <vector>
#include "Layer.h"

//class Layer;
class GameObject;

//	Alles bez�glich der Map befindet sich im Namenraum "Environment"
namespace Environment
{
	//	Instanzen dieser Klasse repr�sentieren unsere Maps.
	class Map
	{
	private:
		Vector2D m_positionVector;						//	Ortsvektor
		std::map<std::string, Layer*> m_layerMap;		//	Dictionary/Map aus zur Map geh�rigen Layern

	public:
		Map();
		~Map();

		void init(std::vector<GameObject*>* pObjects);			//	Initialisieren
		void update();									//	Aktualisieren
		void render();									//	Rendern
	};
}
