#include "ObjectLayer.h"
#include "ParamLoader.h"
#include "TileLayer.h"
#include "Game.h"

Environment::ObjectLayer::ObjectLayer()
	: m_pGameObjects(nullptr),
	  m_pCollisionLayers(new std::vector<TileLayer*>)
{
}

Environment::ObjectLayer::~ObjectLayer()
{
	/*	Nicht zu löschende Pointer:
	 *		Layer in m_pCollisionLayers:
	 *			Weil es Pointer auf Layer der Map sind. 
	 *			Folglich werden die Objekte von der Map gelöscht.
	 *
	 */

	//	Hier wird jedes Spielobjekt iterativ gelöscht
	for (GameObject* g : *m_pGameObjects)
	{
		delete g;
	}

	//	Hier wird der Vector selber gelöscht
	delete m_pGameObjects;

	//	Der Vektor (nicht die Layer) wird gelöscht
	delete m_pCollisionLayers;
}

void Environment::ObjectLayer::init(std::vector<GameObject*>* pGameObjects)
{
	m_pGameObjects = pGameObjects;

	
	ParamLoader playerParams; 

	playerParams.setAnimSpeed(200);
	playerParams.setHeight(180);
	playerParams.setWidth(70);
	playerParams.setNumRows(4);
	playerParams.setNumCols(4);
	playerParams.setTextureId("player");
	playerParams.setX(500.0f);
	playerParams.setY(100.0f);

	m_player.load(playerParams);
	
}

void Environment::ObjectLayer::update()
{
	/*	Jedes Spielobjekt wird iterativ geupdatet
	 *	Außerdem wird bei jedem Objekt gecheckt ob eine Kollision vorliegt
	 */
	for (GameObject* g : *m_pGameObjects)
	{
		g->update();
		objectTileCollision(g);
	}
}

void Environment::ObjectLayer::render()
{
	//	Jedes Spielobjekt wird iterativ gerendert
	for (GameObject* g : *m_pGameObjects)
		g->draw();
}

void Environment::ObjectLayer::objectTileCollision(GameObject* pGameObject)
{
	if (!m_pCollisionLayers->size())
		return;

	//	Es wird gecastet, damit die Funktion 'getVelocity()' aufgerufen werden kann
	SDL_GameObject* pSDLGameObject = dynamic_cast<SDL_GameObject*>(pGameObject);

	//	Ein Zwischenspeicher für die Geschwindigkeit des Objekts
	Vector2D objectVelocity = pSDLGameObject->getVelocity();

	/*	Der Ortsvektor des Objekts wird kopiert.
	 *	Der 'collisionVector' soll in die Richtung/auf den Tile zeigen, 
	 *	in der eine Kollision möglich wäre.
	 */
	Vector2D collisionVector = pSDLGameObject->getCollisionBoxPosition();

	//	Zuerst wird gecheckt ob 
	if (objectVelocity.getLength() > 0)
	{
		//	Es wird über die Kollisionslayer iteriert, um zu checken ob eine Kollision an
		for (auto layer : *m_pCollisionLayers)
		{
			//	Hier wird geschaut, in welche Richtung sich das Objekt im Moment bewegt
			if (objectVelocity.getX() > 0)													//	nach rechts
			{
				//	Die Breite zur x-Komponente des Ortsvektors addieren
				collisionVector.setX(collisionVector.getX() + pSDLGameObject->getCollisionBoxWidth());

				if (rectRectCollisionX(layer, pSDLGameObject, collisionVector))
					pSDLGameObject->collision();
			}
			if (objectVelocity.getX() < 0)													//	nach links
			{
				if (rectRectCollisionX(layer, pSDLGameObject, collisionVector))
					pSDLGameObject->collision();
			}
			if (objectVelocity.getY() > 0)													//	nach unten
			{
				//	Die Höhe zur y-Komponente des Ortsvektors addieren
				collisionVector.setY(collisionVector.getY() + pSDLGameObject->getCollisionBoxHeight());


				if (rectRectCollisionY(layer, pSDLGameObject, collisionVector))
					pSDLGameObject->collision();
			}
			if (objectVelocity.getY() < 0)													//	nach oben
			{
				if (rectRectCollisionY(layer, pSDLGameObject, collisionVector))
					pSDLGameObject->collision();
			}

		}
	}
}

bool Environment::ObjectLayer::rectRectCollisionX(TileLayer* pLayer, SDL_GameObject* pSDLGameObject, Vector2D rectVector)
{

	while (rectVector.getY() < pSDLGameObject->getPosition().getY() + pSDLGameObject->getHeight() + 64)
	{
		if (pLayer->getTileIdAtPosition(rectVector))
		{
			int x = static_cast<int>(rectVector.getX() / 64) * 64 + pLayer->getPosition().getX();
			int y = static_cast<int>(rectVector.getY() / 64) * 64 + pLayer->getPosition().getY();

			int topA, topB;
			int bottomA, bottomB;
			int leftA, leftB;
			int rightA, rightB;

			leftA = pSDLGameObject->getPosition().getX();
			leftB = x;

			rightA = pSDLGameObject->getPosition().getX() + pSDLGameObject->getWidth();
			rightB = x + 64;

			topA = pSDLGameObject->getPosition().getY();
			topB = y;

			bottomA = pSDLGameObject->getPosition().getY() + pSDLGameObject->getHeight();
			bottomB = y + 64;

			bool collision = false;

			if (rightA > leftB && bottomA > topB && topA < bottomB && leftA < rightB)
			{
				collision = true;
			}

			return collision;
		}
		rectVector.setY(rectVector.getY() + 64);
	}
	return false;
}

bool Environment::ObjectLayer::rectRectCollisionY(TileLayer* pLayer, SDL_GameObject* pSDLGameObject, Vector2D rectVector)
{
	while (rectVector.getX() < pSDLGameObject->getPosition().getX() + pSDLGameObject->getWidth() + 64)
	{
		if (pLayer->getTileIdAtPosition(rectVector))
		{
			int x = static_cast<int>(rectVector.getX() / 64) * 64 + pLayer->getPosition().getX();
			int y = static_cast<int>(rectVector.getY() / 64) * 64 + pLayer->getPosition().getY();

			int topA, topB;
			int bottomA, bottomB;
			int leftA, leftB;
			int rightA, rightB;

			leftA = pSDLGameObject->getPosition().getX();
			leftB = x;

			rightA = pSDLGameObject->getPosition().getX() + pSDLGameObject->getWidth();
			rightB = x + 64;

			topA = pSDLGameObject->getPosition().getY();
			topB = y;

			bottomA = pSDLGameObject->getPosition().getY() + pSDLGameObject->getHeight();
			bottomB = y + 64;

			bool collision = false;

			if (rightA > leftB && bottomA > topB && topA < bottomB && leftA < rightB)
			{
				collision = true;
			}

			return collision;
		}
		rectVector.setX(rectVector.getX() + 64);
	}
	return false;
}
