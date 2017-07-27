#pragma once
#include "Layer.h"
#include "Player.h"
#include <vector>

class GameObject;

namespace Environment
{
	class TileLayer;

	/*	Diese Klasse managed unsere "GameObjects".
	 */
	class ObjectLayer : public Layer
	{
	private:
		std::vector<GameObject*>* m_pGameObjects;		//	Array der GameObjects 
		Player m_player;								//	Der Spieler ist Teil des ObjectLayers und wird von diesem gerendert

		std::vector<TileLayer*>* m_pCollisionLayers;			//	Ein Pointer auf alle Layer, mit denen Spielobjekte kollidieren sollen

	public:
		ObjectLayer();
		~ObjectLayer();

		void init(std::vector<GameObject*>* pGameObjects);		//	Initialisieren
		void update();											//	Aktualisieren
		void render();											//	Rendern

		//	setter-Funktion
		void addCollisionLayer(TileLayer* pCollisionLayer) { m_pCollisionLayers->push_back(pCollisionLayer); }

		//	getter-Funktionen
		std::vector<GameObject*>* getGameObjects() const { return m_pGameObjects; }

	private:
		void objectTileCollision(GameObject* pGameObject);
		bool rectRectCollisionX(TileLayer* pLayer, Vector2D rectVector, ObjectRectangle collisionRect);
		bool rectRectCollisionY(TileLayer* pLayer, Vector2D rectVector, ObjectRectangle collisionRect);

	};

	
}
