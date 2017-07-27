#include "ObjectLayer.h"
#include "ParamLoader.h"
#include "TileLayer.h"
#include "Game.h"
#include <algorithm>

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

	// m_player.load(playerParams);
	
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

	std::vector<ObjectRectangle> collisionRects = pSDLGameObject->getCollisonRects();

	for(auto collisionRect : collisionRects)
	{
		/*	Der Ortsvektor des Objekts wird kopiert.
		*	Der 'collisionVector' soll in die Richtung/auf den Tile zeigen,
		*	in der eine Kollision möglich wäre.
		*/
		Vector2D collisionVector = collisionRect.positionVector;

		//	Zuerst wird gecheckt ob das Objekt sich überhaupt bewegt
		if (objectVelocity.getLength() > 0)
		{
			//	Es wird über die Kollisionslayer iteriert, um zu checken ob eine Kollision an
			for (auto layer : *m_pCollisionLayers)
			{
				//	Hier wird geschaut, in welche Richtung sich das Objekt im Moment bewegt
				if (objectVelocity.getX() > 0)													//	nach rechts
				{
					//	Die Breite zur x-Komponente des Ortsvektors addieren
					collisionVector.setX(collisionVector.getX() + collisionRect.getWidth());

					if (rectRectCollisionX(layer, collisionVector, collisionRect))
						pSDLGameObject->collision();
				}
				if (objectVelocity.getX() < 0)													//	nach links
				{
					if (rectRectCollisionX(layer, collisionVector, collisionRect))
						pSDLGameObject->collision();
				}
				if (objectVelocity.getY() > 0)													//	nach unten
				{
					//	Die Höhe zur y-Komponente des Ortsvektors addieren
					collisionVector.setY(collisionVector.getY() + collisionRect.getHeight());


					if (rectRectCollisionY(layer, collisionVector, collisionRect))
						pSDLGameObject->collision();
				}
				if (objectVelocity.getY() < 0)													//	nach oben
				{
					if (rectRectCollisionY(layer, collisionVector, collisionRect))
						pSDLGameObject->collision();
				}

			}
		}
	}
}

bool Environment::ObjectLayer::rectRectCollisionX(TileLayer* pLayer, Vector2D rectVector, ObjectRectangle collisionRect)
{
	bool possibleCollision = true;

	while (rectVector.getY() <= collisionRect.getY() + collisionRect.getHeight())
	{	
		if(pLayer->getTilesets().size())
		{
			int destinationTileId = pLayer->getTileIdAtPosition(rectVector);
		
			Tileset tSet;
			for(int i = 0; i < pLayer->getTilesets().size(); i++)
			{
				if(destinationTileId < pLayer->getTilesets()[i].firstgid + pLayer->getTilesets()[i].tilecount)
				{
					tSet = pLayer->getTilesets()[i];
					break;
				}
			}
		
			std::cout << "Tileset: " << tSet.firstgid << " collisionTileId: " << destinationTileId - tSet.firstgid << " in Collisionmap: " << tSet.collisionMap.count(destinationTileId - tSet.firstgid) << std::endl;
				
			if(tSet.collisionMap.count(destinationTileId - tSet.firstgid))
			{
				for(int i = 0; i < tSet.collisionMap[destinationTileId - tSet.firstgid].size(); i++)
				{
					int x = static_cast<int>(rectVector.getX() / 64) * 64 + pLayer->getPosition().getX();
					int y = static_cast<int>(rectVector.getY() / 64) * 64 + pLayer->getPosition().getY();

					Collisionbox cBox = tSet.collisionMap[destinationTileId - tSet.firstgid][i];

					int topA, topB;
					int bottomA, bottomB;
					int leftA, leftB;
					int rightA, rightB;

					leftA = collisionRect.getX();
					leftB = x + cBox.xPos;

					rightA = collisionRect.getX() + collisionRect.getWidth();
					rightB = leftB + cBox.width;

					topA = collisionRect.getY();
					topB = y + cBox.yPos;

					bottomA = collisionRect.getY() + collisionRect.getHeight();
					bottomB = topB + cBox.height;

					
					if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
					{
						return true;
					}				
				}
				possibleCollision = false;
			}
		}
	
		if(possibleCollision)
		{
			if (pLayer->getTileIdAtPosition(rectVector))
			{
				int x = static_cast<int>(rectVector.getX() / 64) * 64 + pLayer->getPosition().getX();
				int y = static_cast<int>(rectVector.getY() / 64) * 64 + pLayer->getPosition().getY();

				int topA, topB;
				int bottomA, bottomB;
				int leftA, leftB;
				int rightA, rightB;

				leftA = collisionRect.getX();
				leftB = x;

				rightA = collisionRect.getX() + collisionRect.getWidth();
				rightB = x + 64;

				topA = collisionRect.getY();
				topB = y;

				bottomA = collisionRect.getY() + collisionRect.getHeight();
				bottomB = y + 64;


				if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
				{
					return true;
				}
			}
		}
		if(rectVector.getY() + 64 <= collisionRect.getY() + collisionRect.getHeight())
			rectVector.setY(rectVector.getY() + 64);
		else
			rectVector.setY(rectVector.getY() + (collisionRect.getHeight() - (rectVector.getY() - collisionRect.getY())));
	}
	return false;
}

bool Environment::ObjectLayer::rectRectCollisionY(TileLayer* pLayer, Vector2D rectVector, ObjectRectangle collisionRect)
{
	bool possibleCollision = true;

	while (rectVector.getX() <= collisionRect.getX() + collisionRect.getWidth())
	{
		if (pLayer->getTilesets().size())
		{
			int destinationTileId = pLayer->getTileIdAtPosition(rectVector);

			Tileset tSet;
			for (int i = 0; i < pLayer->getTilesets().size(); i++)
			{
				if (destinationTileId < pLayer->getTilesets()[i].firstgid + pLayer->getTilesets()[i].tilecount)
				{
					tSet = pLayer->getTilesets()[i];
					break;
				}
			}

			std::cout << "Tileset: " << tSet.firstgid << " collisionTileId: " << destinationTileId - tSet.firstgid << " in Collisionmap: " << tSet.collisionMap.count(destinationTileId - tSet.firstgid) << std::endl;


			if (tSet.collisionMap.count(destinationTileId - tSet.firstgid))
			{
				for (int i = 0; i < tSet.collisionMap[destinationTileId - tSet.firstgid].size(); i++)
				{
					int x = static_cast<int>(rectVector.getX() / 64) * 64 + pLayer->getPosition().getX();
					int y = static_cast<int>(rectVector.getY() / 64) * 64 + pLayer->getPosition().getY();

					Collisionbox cBox = tSet.collisionMap[destinationTileId - tSet.firstgid][i];

					int topA, topB;
					int bottomA, bottomB;
					int leftA, leftB;
					int rightA, rightB;

					leftA = collisionRect.getX();
					leftB = x + cBox.xPos;

					rightA = collisionRect.getX() + collisionRect.getWidth();
					rightB = leftB + cBox.width;

					topA = collisionRect.getY();
					topB = y + cBox.yPos;

					bottomA = collisionRect.getY() + collisionRect.getHeight();
					bottomB = topB + cBox.height;
					
					if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
					{
						return true;
					}
				}
				possibleCollision = false;
			}
		}

		if(possibleCollision)
		{
			if (pLayer->getTileIdAtPosition(rectVector))
			{
				int x = static_cast<int>(rectVector.getX() / 64) * 64 + pLayer->getPosition().getX();
				int y = static_cast<int>(rectVector.getY() / 64) * 64 + pLayer->getPosition().getY();

				int topA, topB;
				int bottomA, bottomB;
				int leftA, leftB;
				int rightA, rightB;

				leftA = collisionRect.getX();
				leftB = x;

				rightA = collisionRect.getX() + collisionRect.getWidth();
				rightB = x + 64;

				topA = collisionRect.getY();
				topB = y;

				bottomA = collisionRect.getY() + collisionRect.getHeight();
				bottomB = y + 64;

				if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
				{
					return true;
				}
			}
		}
		if (rectVector.getX() + 64 <= collisionRect.getX() + collisionRect.getWidth())
			rectVector.setX(rectVector.getX() + 64);
		else
			rectVector.setX(rectVector.getX() + (collisionRect.getWidth() - (rectVector.getX() - collisionRect.getX())));
	}
	return false;
}