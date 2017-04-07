#pragma once
#include "Layer.h"
#include "Player.h"
#include <vector>

class GameObject;

namespace Environment
{
	/*	Diese Klasse managed unsere "GameObjects".
	 */
	class ObjectLayer : public Layer
	{
	private:
		std::vector<GameObject*> m_gameObjects;		//	Array der GameObjects 
		Player m_player;							//	Der Spieler ist Teil des ObjectLayers und wird von diesem gerendert.

	public:
		ObjectLayer();
		~ObjectLayer();

		void init();							//	Initialisieren
		void update();							//	Aktualisieren
		void render();							//	Rendern
	};

	
}
