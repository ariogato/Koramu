#include "ObjectLayer.h"
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
}

void Environment::ObjectLayer::update()
{
	/*	Jedes Spielobjekt wird iterativ geupdatet
	 *	Außerdem wird bei jedem Objekt gecheckt, ob eine Kollision vorliegt
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
	//	Wenn keine "CollisionLayer" existieren, muss auch nicht auf Kollision mit Tiles überprüft werden
	if (!m_pCollisionLayers->size())
		return;
	
	//	Besitzt das Objekt keine Kolllisionsboxen, so kann es auch nicht kollidieren
	if (!pGameObject->getCollisionRects().size())
		return;

	//	Ein Zwischenspeicher für die Geschwindigkeit des Objekts
	Vector2D objectVelocity = pGameObject->getVelocity();

	//	Zuerst wird gecheckt, ob das Objekt sich überhaupt bewegt
	if (objectVelocity.getLength() > 0)
	{
		//	Es wird über die Kollisionsrectecke des Objekts iteriert, um jedes einzelne auf Kollision zu übeprüfen
		for(auto collisionRect : pGameObject->getCollisionRects())
		{
			/*	Der Ortsvektor des Objekts wird kopiert.
			*	Der 'collisionVector' soll in die Richtung/auf den Tile zeigen,
			*	in der eine Kollision möglich wäre.
			*/
			Vector2D collisionVector = collisionRect.positionVector;
				
			//	Es wird über die Kollisionslayer iteriert, um zu checken ob eine Kollision mit einem Tile auf dem Kollisionlayer vorliegt
			for (auto layer : *m_pCollisionLayers)
			{
					//	Hier wird geschaut, in welche Richtung sich das Objekt im Moment bewegt
					if (objectVelocity.getX() > 0)													//	nach rechts
					{
						//	Die Breite zur x-Komponente des Ortsvektors addieren
						collisionVector.setX(collisionVector.getX() + collisionRect.getWidth());

						//	Auf Kollision in x-Richtung überprüfen und entsprechend reagieren
						if (rectRectCollisionX(layer, collisionVector, &collisionRect))
						{
							pGameObject->collision();
							return;
						}
					}
					if (objectVelocity.getX() < 0)													//	nach links
					{
						//	Auf Kollision in x-Richtung überprüfen und entsprechend reagieren
						if (rectRectCollisionX(layer, collisionVector, &collisionRect))
						{
							pGameObject->collision();
							return;
						}
					}
					if (objectVelocity.getY() > 0)													//	nach unten
					{
						//	Die Höhe zur y-Komponente des Ortsvektors addieren
						collisionVector.setY(collisionVector.getY() + collisionRect.getHeight());

						//	Auf Kollision in y-Richtung überprüfen und entsprechend reagieren
						if (rectRectCollisionY(layer, collisionVector, &collisionRect))
						{
							pGameObject->collision();
							return;
						}
					}
					if (objectVelocity.getY() < 0)													//	nach oben
					{
						//	Auf Kollision in y-Richtung überprüfen und entsprechend reagieren
						if (rectRectCollisionY(layer, collisionVector, &collisionRect))
						{
							pGameObject->collision();
							return;
						}
					}

				}
			}
	}
}

bool Environment::ObjectLayer::rectRectCollisionX(TileLayer* pLayer, Vector2D rectVector, ObjectRectangle* collisionRect)
{
	//	Überprüfen, ob das aktuelle "Layer" überhaupt Tiles hat
	if (pLayer->getTilesets().size())
	{
		//	Der "rectVector" wird so lange in y-Richtung verschoben, bis er auf alle für die Kollisionerkennung relevanten Tiles gezeigt hat
		while (rectVector.getY() <= collisionRect->getY() + collisionRect->getHeight())
		{
			/*	Variable, die festhält, ob auf Kollision mit dem gesamten Tile überprüft werden muss.
			 *	Dies soll nicht geschehen, wenn für das Tile Kollisionsboxen festgelegt wurden ("Tiled")
			 */
			bool collisionWithEntireTile = true;

			//	Id des Tiles, auf das der "rectVector" zeigt
			int destinationTileId = pLayer->getTileIdAtPosition(rectVector);

			/*	Überprüfen, ob sich, in dem übergebenen Kollisionslayer, überhaupt ein Tile an dieser Position befindet.
			 *	"destinationTileId" ist 0, wenn dies nicht der Fall ist
			 */
			if(destinationTileId)
			{
				//	Ermmitteln des Tilesets, auf dem sich das betreffende Tile befindet
				Tileset tSet;
				for (int i = 0; i < pLayer->getTilesets().size(); i++)
				{
					if (destinationTileId < pLayer->getTilesets()[i].firstgid + pLayer->getTilesets()[i].tilecount)
					{
						tSet = pLayer->getTilesets()[i];
						break;
					}
				}

				//	x- und y-Position des Tiles berechnen
				int x = static_cast<int>(rectVector.getX() / 64) * 64 + pLayer->getPosition().getX();
				int y = static_cast<int>(rectVector.getY() / 64) * 64 + pLayer->getPosition().getY();

				//	Definition von Variablen zur Speicherung der Extremwerte der zu vergleichenden Rechtecke (axis-aligned - entlang der Achsen ausgerichtet)
				int topA, topB;						//	y-Wert von Punkten auf der oberen Kante
				int bottomA, bottomB;				//	y-Wert von Punkten auf der unteren Kante
				int leftA, leftB;					//	x-Wert von Punkten auf der linken Kante
				int rightA, rightB;					//	x-Wert von Punkten auf der rechten Kante

				//	Überprüfen, ob das Tile, in "Tiled" festgelegte, Kollisionsboxen besitzt
				if (tSet.collisionMap.count(destinationTileId - tSet.firstgid))
				{
					//	Das Tile besitzt Kollisionboxen - es muss nur noch die Kollision mit diesen überprüft werden
					collisionWithEntireTile = false;

					//	Über die Kollisionsboxen des Tiles iterieren
					for (int i = 0; i < tSet.collisionMap[destinationTileId - tSet.firstgid].size(); i++)
					{
						//	Aktuelle Kollisionsbox kopieren
						Collisionbox cBox = tSet.collisionMap[destinationTileId - tSet.firstgid][i];

						/*	Im folgenden werden die Extremwerte der zu vegleichenden Rechtecke ermittelt.
						*
						*	Zu vergleichen sind das übergebene Kollisionrechteck des Spielobjekts ("collisionRect")
						*	und die aktuelle Kollisionsbox ("cBox").
						*
						*	x- und y-Position der Kollisionsbox sind relativ zur Position des Tiles gespeichert
						*/
						leftA = collisionRect->getX();
						leftB = x + cBox.xPos;

						rightA = collisionRect->getX() + collisionRect->getWidth();
						rightB = leftB + cBox.width;

						topA = collisionRect->getY();
						topB = y + cBox.yPos;

						bottomA = collisionRect->getY() + collisionRect->getHeight();
						bottomB = topB + cBox.height;

						//	Wenn folgende Bedingungen alle zutrefen, dann berühren oder überlappen sich die Rechtecke - es liegt eine Kollision vor
						if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
						{
							return true;
						}
					}
				}

				//	Ermitteln, ob noch auf Kollsion mit dem gesamten Tile überprüft werden soll
				if (collisionWithEntireTile)
				{
						/*	Im folgenden werden die Extremwerte der zu vegleichenden Rechtecke ermittelt.
						*
						*	Zu vergleichen sind das übergebene Kollisionrechteck des Spielobjekts ("collisionRect")
						*	und das aktuelle Tile (64px x 64px).
						*/
						leftA = collisionRect->getX();
						leftB = x;

						rightA = collisionRect->getX() + collisionRect->getWidth();
						rightB = x + 64;

						topA = collisionRect->getY();
						topB = y;

						bottomA = collisionRect->getY() + collisionRect->getHeight();
						bottomB = y + 64;

						//	Wenn folgende Bedingungen alle zutrefen, dann berühren oder überlappen sich die Rechtecke - es liegt eine Kollision vor
						if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
						{
							return true;
						}
				}
			}
			/*	Der "rectVector" wird solange es möglich ist in 64er-Schritten nach unten verschoben.
			*
			*	Dannach wird er um genau den Wert nach unten verschoben, der zur unteren Kannte des "collisionRect"s fehlt.
			*
			*	Damit wir keine Endlosschleife erzeugen, verschiebem wir den "rectVector" im nächsten Schleifendurchlauf um 1 nach unten.
			*/
			if (rectVector.getY() + 64 <= collisionRect->getY() + collisionRect->getHeight())
				rectVector.setY(rectVector.getY() + 64);
			else if (rectVector.getY() == collisionRect->getY() + collisionRect->getHeight())
				rectVector.setY(rectVector.getY() + 1);
			else
				rectVector.setY(rectVector.getY() + (collisionRect->getHeight() - (rectVector.getY() - collisionRect->getY())));
		}
	}
	//	Es liegt keine Kollision vor
	return false;
}

bool Environment::ObjectLayer::rectRectCollisionY(TileLayer* pLayer, Vector2D rectVector, ObjectRectangle* collisionRect)
{
	//	Überprüfen, ob das aktuelle "Layer" überhaupt Tiles hat
	if (pLayer->getTilesets().size())
	{
		//	Der "rectVector" wird so lange in x-Richtung verschoben, bis er auf alle für die Kollisionerkennung relevanten Tiles gezeigt hat
		while (rectVector.getX() <= collisionRect->getX() + collisionRect->getWidth())
		{

			/*	Variable, die festhält, ob auf Kollision mit dem gesamten Tile überprüft werden muss.
			*	Dies soll nicht geschehen, wenn für das Tile Kollisionsboxen festgelegt wurden ("Tiled")
			*/
			bool collisionWithEntireTile = true;

			//	Id des Tiles, auf das der "rectVector" zeigt
			int destinationTileId = pLayer->getTileIdAtPosition(rectVector);

			/*	Überprüfen, ob sich in dem übergebenen Kollisionslayer überhaupt ein Tile an dieser Position befindet.
			*	"destinationTileId" ist 0, wenn dies nicht der Fall ist
			*/
			if (destinationTileId)
			{
				//	Ermmitteln des Tilesets, auf dem sich das betreffende Tile befindet
				Tileset tSet;
				for (int i = 0; i < pLayer->getTilesets().size(); i++)
				{
					if (destinationTileId < pLayer->getTilesets()[i].firstgid + pLayer->getTilesets()[i].tilecount)
					{
						tSet = pLayer->getTilesets()[i];
						break;
					}
				}

				//	x- und y-Position des Tiles berechnen
				int x = static_cast<int>(rectVector.getX() / 64) * 64 + pLayer->getPosition().getX();
				int y = static_cast<int>(rectVector.getY() / 64) * 64 + pLayer->getPosition().getY();

				//	Definition von Variablen zur Speicherung der Extremwerte der zu vergleichenden Rechtecke (axis-aligned - entlang der Achsen ausgerichtet)
				int topA, topB;						//	y-Wert von Punkten auf der oberen Kante
				int bottomA, bottomB;				//	y-Wert von Punkten auf der unteren Kante
				int leftA, leftB;					//	x-Wert von Punkten auf der linken Kante
				int rightA, rightB;					//	x-Wert von Punkten auf der rechten Kante

				//	Überprüfen, ob das Tile, in "Tiled" festgelegte, Kollisionsboxen besitzt
				if (tSet.collisionMap.count(destinationTileId - tSet.firstgid))
				{
					//	Das Tile besitzt Kollisionboxen - es muss nur noch die Kollision mit diesen überprüft werden
					collisionWithEntireTile = false;

					//	Über die Kollisionsboxen des Tiles iterieren
					for (int i = 0; i < tSet.collisionMap[destinationTileId - tSet.firstgid].size(); i++)
					{
						//	Aktuelle Kollisionsbox kopieren
						Collisionbox cBox = tSet.collisionMap[destinationTileId - tSet.firstgid][i];

						/*	Im folgenden werden die Extremwerte der zu vegleichenden Rechtecke ermittelt.
						*
						*	Zu vergleichen sind das übergebene Kollisionrechteck des Spielobjekts ("collisionRect")
						*	und die aktuelle Kollisionsbox ("cBox").
						*
						*	x- und y-Position der Kollisionsbox sind relativ zur Position des Tiles gespeichert
						*/
						leftA = collisionRect->getX();
						leftB = x + cBox.xPos;

						rightA = collisionRect->getX() + collisionRect->getWidth();
						rightB = leftB + cBox.width;

						topA = collisionRect->getY();
						topB = y + cBox.yPos;

						bottomA = collisionRect->getY() + collisionRect->getHeight();
						bottomB = topB + cBox.height;

						//	Wenn folgende Bedingungen alle zutrefen, dann berühren oder überlappen sich die Rechtecke - es liegt eine Kollision vor
						if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
						{
							return true;
						}
					}
				}

				//	Ermitteln, ob noch auf Kollsion mit dem gesamten Tile überprüft werden soll
				if (collisionWithEntireTile)
				{
					/*	Im folgenden werden die Extremwerte der zu vegleichenden Rechtecke ermittelt.
					*
					*	Zu vergleichen sind das übergebene Kollisionrechteck des Spielobjekts ("collisionRect")
					*	und das aktuelle Tile (64px x 64px).
					*/
					leftA = collisionRect->getX();
					leftB = x;

					rightA = collisionRect->getX() + collisionRect->getWidth();
					rightB = x + 64;

					topA = collisionRect->getY();
					topB = y;

					bottomA = collisionRect->getY() + collisionRect->getHeight();
					bottomB = y + 64;

					//	Wenn folgende Bedingungen alle zutrefen, dann berühren oder überlappen sich die Rechtecke - es liegt eine Kollision vor
					if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
					{
						return true;
					}
				}
			}
			/*	Der "rectVector" wird solange es möglich ist in 64er-Schritten nach rechts verschoben.
			*
			*	Dannach wird er um genau den Wert nach rechts verschoben, der zur rechten Kannte des "collisionRects" fehlt.
			*
			*	Damit wir keine Endlosschleife erzeugen, verschiebem wir den "rectVector" im nächsten Schleifendurchlauf um 1 nach rechts.
			*/
			if (rectVector.getX() + 64 <= collisionRect->getX() + collisionRect->getWidth())
				rectVector.setX(rectVector.getX() + 64);
			else if (rectVector.getX() == collisionRect->getX() + collisionRect->getWidth())
				rectVector.setX(rectVector.getX() + 1);
			else
				rectVector.setX(rectVector.getX() + (collisionRect->getWidth() - (rectVector.getX() - collisionRect->getX())));
		}
	}
	//	Es liegt keine Kollision vor
	return false;
}