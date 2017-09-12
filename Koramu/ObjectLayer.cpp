#include "ObjectLayer.h"
#include "TileLayer.h"
#include "Game.h"
#include "Camera.h"
#include <algorithm>

Environment::ObjectLayer::ObjectLayer()
	: m_pGameObjects(nullptr),
	  m_pCollisionLayers(new std::vector<TileLayer*>)
{
}

Environment::ObjectLayer::~ObjectLayer()
{
	/*	Nicht zu l�schende Pointer:
	 *		Layer in m_pCollisionLayers:
	 *			Weil es Pointer auf Layer der Map sind. 
	 *			Folglich werden die Objekte von der Map gel�scht.
	 *
	 */

	//	Hier wird jedes Spielobjekt iterativ gel�scht
	for (GameObject* g : *m_pGameObjects)
	{
		delete g;
	}

	//	Hier wird der Vector selber gel�scht
	delete m_pGameObjects;

	//	Der Vektor (nicht die Layer) wird gel�scht
	delete m_pCollisionLayers;
}

void Environment::ObjectLayer::init(std::vector<GameObject*>* pGameObjects)
{
	m_pGameObjects = pGameObjects;
}

void Environment::ObjectLayer::update()
{
	//	Array, in welchem Objekte gespeichert werden, die aktuell in Bewegung sind (f�r "objectObjectCollision")
	std::vector<GameObject*> movingObjects;
		
	/*	Jedes Spielobjekt wird iterativ geupdatet
	 *	Au�erdem wird bei jedem, sich bewegenden, Objekt gecheckt, ob eine Kollision vorliegt
	 */
	for (GameObject* g : *m_pGameObjects)
	{
		g->update();

		//	Es wird gecheckt, ob das Objekt in Bewegung ist
		if(g->getVelocity().getLength())
		{
			//	Das Objekt bewegt sich und wird daher in "movingObjects" eingetragen
			movingObjects.push_back(g);

			//	Das, sich bewegende, Objekt wird auf Kollision mit Tiles �berpr�ft
			objectTileCollision(g);
		}
	}

	//	Die Objekte in Bewegung ("movingObjects") werden auf Kollision mit anderen Objekten �berpr�ft
	objectObjectCollison(&movingObjects);

	/*	Der nachfolgende Code ist f�r das sogenannte "Z-ordering" zust�ndig.
	 *	Wieso wir das machen und was das genau bedeutet, ist in Ticket #34 "Z-Order Rendering" festgehalten.
	 *	
	 *	Die Spielobjekte ("m_pGameObjects") werden aufsteigend nach dem am weitesten unten liegenden Punkt (gr��ter y-Wert)
	 *	ihrer Kollisionsrechtecke sortiert. Ein kleinerer y-Wert bedeutet, dass sie weiter vorne im "std::vector" stehen und
	 *	demnach zuerst gerendert werden. Dadurch entsteht f�r den Betrachter der Eindruck von Tiefe.
	 *	
	 *	Die Sortierung wird erreicht, indem wir "std::sort()" �ber den "std::vector" iterieren lassen und im dritten Parameter
	 *	eine (Lambda-)Funktion definieren, nach der geordnet wird.
	 */
	std::sort(m_pGameObjects->begin(), m_pGameObjects->end(), [](const GameObject* objectA, const GameObject* objectB)
	{
		//	Kollisionsrechtecke der zu vergleichenden Objekte (objectA und objectB) holen
		std::vector<ObjectRectangle> collisionRectsA = objectA->getCollisionRects();
		std::vector<ObjectRectangle> collisionRectsB = objectB->getCollisionRects();

		//	Speicher f�r den maximalen y-Wert der Kollisionsrechtecke der beiden Objekte
		int maxValueA = 0; 
		int maxValueB = 0;

		//	�ber die Kollisonsrechtecke von "objectA" iterieren
		for(auto collisionRect : collisionRectsA)
		{
			/*	Ist der maximale y-Wert des aktuellen Kollisionsrechtecks gr��er, 
			 *	als der bisher ermittelte Maximalwert, so wird er als neuer Maximalwert festgelegt
			 */
			if( (collisionRect.getY() + collisionRect.getHeight()) > maxValueA)
			{
				maxValueA = (collisionRect.getY() + collisionRect.getHeight());
			}
		}

		//	�ber die Kollisonsrechtecke von "objectB" iterieren
		for (auto collisionRect : collisionRectsB)
		{
			/*	Ist der maximale y-Wert des aktuellen Kollisionsrechtecks gr��er,
			*	als der bisher ermittelte Maximalwert, so wird er als neuer Maximalwert festgelegt
			*/
			if ((collisionRect.getY() + collisionRect.getHeight()) > maxValueB)
			{
				maxValueB = (collisionRect.getY() + collisionRect.getHeight());
			}
		}

		/*	Vergleichen der beiden Maximalwerte und R�ckgabe eines f�r "std::sort()" verwertbaren Wahrheitswertes
		 *	
		 *	(Das "<"-Zeichen sorgt f�r eine aufsteigende Sortierung, ">" h�tte eine absteigende Sortierung zur Folge)
		 */
		return maxValueA < maxValueB;
	});
}

void Environment::ObjectLayer::render()
{
	/*	Im Folgenden wird f�r jedes Spielobjekt des "ObjectLayer"s �berpr�ft, ob es sich (zumindest teilweise)
	 *	innerhalb des durch die Kamera festgelegten Rechtecks befindet.
	 *	Nur Spielobjekte, f�r die das der Fall ist sollen gezeichnet werden. Die anderen sind f�r den Benutzer sowieso nicht sichtbar.
	 *	
	 *	Diese �berpr�fung �hnelt stark der Kollisionserkennung. Betrachtet werden hier die "ObjectRectangle"s der Spielobjekte und das, durch die 
	 *	Kamera festgelegte Rechteck. F�r weitere Informationen siehe unten.
	 */

	//	Definition von Variablen zur Speicherung der Extremwerte der zu vergleichenden Rechtecke (axis-aligned - entlang der Achsen ausgerichtet)
	int topA, topB;						//	y-Wert von Punkten auf der oberen Kante
	int bottomA, bottomB;				//	y-Wert von Punkten auf der unteren Kante
	int leftA, leftB;					//	x-Wert von Punkten auf der linken Kante
	int rightA, rightB;					//	x-Wert von Punkten auf der rechten Kante

	//	Extremwerte des durch die Kamera festgelegten Rechtecks
	topA = TheGame::Instance()->getCamera()->getPositionVector().getY();
	bottomA = topA + TheGame::Instance()->getCamera()->getCameraHeight();
	leftA = TheGame::Instance()->getCamera()->getPositionVector().getX();
	rightA = leftA + TheGame::Instance()->getCamera()->getCameraWidth();

	//	Es wird �ber alle Spielobjekte iteriert und nach obiger Bedingung ggf. gerendert
	for (GameObject* g : *m_pGameObjects)
	{
		//	Extremwerte, des "ObjectRectangle"s des Spielobjekts
		topB = g->getPosition().getY();
		bottomB = topB + g->getHeight();
		leftB = g->getPosition().getX();
		rightB = leftB + g->getWidth();
		
		//	Wenn folgende Bedingungen alle zutrefen, dann ber�hren oder �berlappen sich die Rechtecke - das Spielobjekt soll gezeichnet werden
		if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
		{
			g->draw(m_positionVector);
		}

	}
}


/*	Die nachfolgenden vier Methoden besch�ftigen sich mit der Kollisonserkennung. 
 *	F�r grundlegenede Informationen hierzu siehe: https://github.com/ariogato/Koramu/wiki/Unsere-Spielkonzepte -> Collision Detection
 */

void Environment::ObjectLayer::objectTileCollision(GameObject* pGameObject)
{
	//	Wenn keine "CollisionLayer" existieren, muss auch nicht auf Kollision mit Tiles �berpr�ft werden
	if (!m_pCollisionLayers->size())
		return;
	
	//	Besitzt das Objekt keine Kollisionsboxen, so kann es auch nicht kollidieren
	if (!pGameObject->getCollisionRects().size())
		return;

	//	Ein Zwischenspeicher f�r die Geschwindigkeit des Objekts
	Vector2D objectVelocity = pGameObject->getVelocity();

	//	Es wird �ber die Kollisionsrechtecke des Objekts iteriert, um jedes einzelne auf Kollision zu �berpr�fen
	for(auto collisionRect : pGameObject->getCollisionRects())
	{
		/*	Der Ortsvektor des Objekts wird kopiert.
		*	Der 'collisionVector' soll in die Richtung/auf den Tile zeigen,
		*	in der eine Kollision m�glich w�re.
		*/
		Vector2D collisionVector = collisionRect.positionVector;
				
		//	Es wird �ber die Kollisionslayer iteriert, um zu checken, ob eine Kollision mit einem Tile auf dem Kollisionlayer vorliegt
		for (auto layer : *m_pCollisionLayers)
		{
				//	Hier wird geschaut, in welche Richtung sich das Objekt im Moment bewegt
				if (objectVelocity.getX() > 0)													//	nach rechts
				{
					//	Die Breite zur x-Komponente des Ortsvektors addieren
					collisionVector.setX(collisionVector.getX() + collisionRect.getWidth());

					//	Auf Kollision in x-Richtung �berpr�fen und entsprechend reagieren
					if (rectRectCollisionX(layer, collisionVector, &collisionRect))
					{
						pGameObject->collision();
						return;
					}
				}
				if (objectVelocity.getX() < 0)													//	nach links
				{
					//	Auf Kollision in x-Richtung �berpr�fen und entsprechend reagieren
					if (rectRectCollisionX(layer, collisionVector, &collisionRect))
					{
						pGameObject->collision();
						return;
					}
				}
				if (objectVelocity.getY() > 0)													//	nach unten
				{
					//	Die H�he zur y-Komponente des Ortsvektors addieren
					collisionVector.setY(collisionVector.getY() + collisionRect.getHeight());

					//	Auf Kollision in y-Richtung �berpr�fen und entsprechend reagieren
					if (rectRectCollisionY(layer, collisionVector, &collisionRect))
					{
						pGameObject->collision();
						return;
					}
				}
				if (objectVelocity.getY() < 0)													//	nach oben
				{
					//	Auf Kollision in y-Richtung �berpr�fen und entsprechend reagieren
					if (rectRectCollisionY(layer, collisionVector, &collisionRect))
					{
						pGameObject->collision();
						return;
					}
				}

			}
		}
}

bool Environment::ObjectLayer::rectRectCollisionX(TileLayer* pLayer, Vector2D rectVector, ObjectRectangle* collisionRect)
{
	//	�berpr�fen, ob das aktuelle "Layer" �berhaupt Tiles hat
	if (pLayer->getTilesets().size())
	{
		//	Der "rectVector" wird so lange in y-Richtung verschoben, bis er auf alle f�r die Kollisionerkennung relevanten Tiles gezeigt hat
		while (rectVector.getY() <= collisionRect->getY() + collisionRect->getHeight())
		{
			/*	Variable, die festh�lt, ob auf Kollision mit dem gesamten Tile �berpr�ft werden muss.
			 *	Dies soll nicht geschehen, wenn f�r das Tile Kollisionsboxen festgelegt wurden ("Tiled")
			 */
			bool collisionWithEntireTile = true;

			//	Id des Tiles, auf das der "rectVector" zeigt
			int destinationTileId = pLayer->getTileIdAtPosition(rectVector);

			/*	�berpr�fen, ob sich, in dem �bergebenen Kollisionslayer, �berhaupt ein Tile an dieser Position befindet.
			 *	"destinationTileId" ist 0, wenn dies nicht der Fall ist
			 */
			if(destinationTileId)
			{
				//	Ermitteln des Tilesets, auf dem sich das betreffende Tile befindet
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
				int x = static_cast<int>(rectVector.getX() / 64) * 64;
				int y = static_cast<int>(rectVector.getY() / 64) * 64;

				//	Definition von Variablen zur Speicherung der Extremwerte der zu vergleichenden Rechtecke (axis-aligned - entlang der Achsen ausgerichtet)
				int topA, topB;						//	y-Wert von Punkten auf der oberen Kante
				int bottomA, bottomB;				//	y-Wert von Punkten auf der unteren Kante
				int leftA, leftB;					//	x-Wert von Punkten auf der linken Kante
				int rightA, rightB;					//	x-Wert von Punkten auf der rechten Kante

				//	�berpr�fen, ob das Tile, in "Tiled" festgelegte, Kollisionsboxen besitzt
				if (tSet.collisionMap.count(destinationTileId - tSet.firstgid))
				{
					//	Das Tile besitzt Kollisionboxen - es muss nur noch die Kollision mit diesen �berpr�ft werden
					collisionWithEntireTile = false;

					//	�ber die Kollisionsboxen des Tiles iterieren
					for (int i = 0; i < tSet.collisionMap[destinationTileId - tSet.firstgid].size(); i++)
					{
						//	Aktuelle Kollisionsbox kopieren
						Collisionbox cBox = tSet.collisionMap[destinationTileId - tSet.firstgid][i];

						/*	Im folgenden werden die Extremwerte der zu vergleichenden Rechtecke ermittelt.
						*
						*	Zu vergleichen sind das �bergebene Kollisionsrechteck des Spielobjekts ("collisionRect")
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

						//	Wenn folgende Bedingungen alle zutrefen, dann ber�hren oder �berlappen sich die Rechtecke - es liegt eine Kollision vor
						if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
						{
							return true;
						}
					}
				}

				//	Ermitteln, ob noch auf Kollision mit dem gesamten Tile �berpr�ft werden soll
				if (collisionWithEntireTile)
				{
						/*	Im folgenden werden die Extremwerte der zu vegleichenden Rechtecke ermittelt.
						*
						*	Zu vergleichen sind das �bergebene Kollisionsrechteck des Spielobjekts ("collisionRect")
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

						//	Wenn folgende Bedingungen alle zutreffen, dann ber�hren oder �berlappen sich die Rechtecke - es liegt eine Kollision vor
						if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
						{
							return true;
						}
				}
			}
			/*	Der "rectVector" wird solange es m�glich ist in 64er-Schritten nach unten verschoben.
			*
			*	Danach wird er um genau den Wert nach unten verschoben, der zur unteren Kannte des "collisionRect"s fehlt.
			*
			*	Damit wir keine Endlosschleife erzeugen, verschieben wir den "rectVector" im n�chsten Schleifendurchlauf um 1 nach unten.
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
	//	�berpr�fen, ob das aktuelle "Layer" �berhaupt Tiles hat
	if (pLayer->getTilesets().size())
	{
		//	Der "rectVector" wird so lange in x-Richtung verschoben, bis er auf alle f�r die Kollisionserkennung relevanten Tiles gezeigt hat
		while (rectVector.getX() <= collisionRect->getX() + collisionRect->getWidth())
		{

			/*	Variable, die festh�lt, ob auf Kollision mit dem gesamten Tile �berpr�ft werden muss.
			*	Dies soll nicht geschehen, wenn f�r das Tile Kollisionsboxen festgelegt wurden ("Tiled")
			*/
			bool collisionWithEntireTile = true;

			//	Id des Tiles, auf das der "rectVector" zeigt
			int destinationTileId = pLayer->getTileIdAtPosition(rectVector);

			/*	�berpr�fen, ob sich in dem �bergebenen Kollisionslayer �berhaupt ein Tile an dieser Position befindet.
			*	"destinationTileId" ist 0, wenn dies nicht der Fall ist
			*/
			if (destinationTileId)
			{
				//	Ermitteln des Tilesets, auf dem sich das betreffende Tile befindet
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
				int x = static_cast<int>(rectVector.getX() / 64) * 64;
				int y = static_cast<int>(rectVector.getY() / 64) * 64;

				//	Definition von Variablen zur Speicherung der Extremwerte der zu vergleichenden Rechtecke (axis-aligned - entlang der Achsen ausgerichtet)
				int topA, topB;						//	y-Wert von Punkten auf der oberen Kante
				int bottomA, bottomB;				//	y-Wert von Punkten auf der unteren Kante
				int leftA, leftB;					//	x-Wert von Punkten auf der linken Kante
				int rightA, rightB;					//	x-Wert von Punkten auf der rechten Kante

				//	�berpr�fen, ob das Tile, in "Tiled" festgelegte, Kollisionsboxen besitzt
				if (tSet.collisionMap.count(destinationTileId - tSet.firstgid))
				{
					//	Das Tile besitzt Kollisionsboxen - es muss nur noch die Kollision mit diesen �berpr�ft werden
					collisionWithEntireTile = false;

					//	�ber die Kollisionsboxen des Tiles iterieren
					for (int i = 0; i < tSet.collisionMap[destinationTileId - tSet.firstgid].size(); i++)
					{
						//	Aktuelle Kollisionsbox kopieren
						Collisionbox cBox = tSet.collisionMap[destinationTileId - tSet.firstgid][i];

						/*	Im folgenden werden die Extremwerte der zu vergleichenden Rechtecke ermittelt.
						*
						*	Zu vergleichen sind das �bergebene Kollisionsrechteck des Spielobjekts ("collisionRect")
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

						//	Wenn folgende Bedingungen alle zutreffen, dann ber�hren oder �berlappen sich die Rechtecke - es liegt eine Kollision vor
						if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
						{
							return true;
						}
					}
				}

				//	Ermitteln, ob noch auf Kollision mit dem gesamten Tile �berpr�ft werden soll
				if (collisionWithEntireTile)
				{
					/*	Im folgenden werden die Extremwerte der zu vegleichenden Rechtecke ermittelt.
					*
					*	Zu vergleichen sind das �bergebene Kollisionsrechteck des Spielobjekts ("collisionRect")
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

					//	Wenn folgende Bedingungen alle zutrefen, dann ber�hren oder �berlappen sich die Rechtecke - es liegt eine Kollision vor
					if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
					{
						return true;
					}
				}
			}
			/*	Der "rectVector" wird solange es m�glich ist in 64er-Schritten nach rechts verschoben.
			*
			*	Danach wird er um genau den Wert nach rechts verschoben, der zur rechten Kante des "collisionRect"s fehlt.
			*
			*	Damit wir keine Endlosschleife erzeugen, verschieben wir den "rectVector" im n�chsten Schleifendurchlauf um 1 nach rechts.
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

void Environment::ObjectLayer::objectObjectCollison(std::vector<GameObject*>* pMovingObjects)
{
	//	Definition von Variablen zur Speicherung der Extremwerte der zu vergleichenden Rechtecke (axis-aligned - entlang der Achsen ausgerichtet)
	int topA, topB;						//	y-Wert von Punkten auf der oberen Kante
	int bottomA, bottomB;				//	y-Wert von Punkten auf der unteren Kante
	int leftA, leftB;					//	x-Wert von Punkten auf der linken Kante
	int rightA, rightB;					//	x-Wert von Punkten auf der rechten Kante

	//	�ber die �bergebenen, sich bewegenden, Objekte iterieren
	for(GameObject* gA : *pMovingObjects)
	{
		//	Variable, die festh�lt, ob bereits eine Kollision f�r das aktuelle Objekt festgestellt wurde
		bool collision = false;

		//	�ber die Kollisionsrechtecke des aktuellen Spielobjekts iterieren
		for (auto collisionRectA : gA->getCollisionRects())
		{
			//	Extremwerte des aktuellen Kollisionsrechtecks ermitteln
			topA = collisionRectA.getY();
			bottomA = collisionRectA.getY() + collisionRectA.getHeight();
			leftA = collisionRectA.getX();
			rightA = collisionRectA.getX() + collisionRectA.getWidth();

			//	Hard gecodete Extremwerte der Map - Todo: schlauer implementieren
			topB = 0;
			bottomB = 5120;
			leftB = 0;
			rightB = 6400;

			//	�berpr�fen, ob das Spielobjekt sich �ber den Rand der Map bewegen will
			if (topA < topB || bottomA > bottomB || leftA < leftB || rightA > rightB)
			{
				//	Kollision ausl�sen
				gA->collision();
				//	Festhalten, dass das Spielobjekt bereits kollidiert ist
				collision = true;
			}

			//	�berpr�fen, ob das Spielobjekt schon kollidiert ist, wenn ja, muss nicht mehr auf Kollision �berpr�ft werden
			if(!collision)
			{
				/*	�ber alle Spielobjekte iterieren, um das aktuelle Kollisionsrechteck auf Kollision mit den Kollisionsrechtecken dieser zu �berpr�fen.
				*
				*	Dies ist sehr naiv und ineffizient, da die meisten Objekte viel zu weit weg sind, als dass es �berhaupt Sinn macht, auf Kollision mit ihnen zu �berpr�fen.
				*	Hier muss man sich bei evtl. auftretenden Performanzproblemen noch etwas geschickteres einfallen lassen (Idee: Quad Trees).
				*/
				for (GameObject* gB : *m_pGameObjects)
				{
					//	Wenn das Objekt das selbe ist, wie das aktuelle Objekt aus "pMovingObjekts", soll mit dem n�chsten Objekt weiter gemacht werden
					if (gA == gB)
						continue;

				//	�ber die Kollisionsrechtecke des Spielobjekts "gB" iterieren
				for(auto collisionRectB : gB->getCollisionRects())
				{
					/*	Im folgenden werden die Extremwerte der zu vergleichenden Rechtecke ermittelt.
					 *	
					 *	Zu vergleichen sind "collisionRectA", das aktuell betrachtete Kollisionsrechteck von "gA"
					 *	und "collsionRectB", das aktuell betrachtete Kollisionsrechteck von "gB"
					 */
					leftA = collisionRectA.getX();
					leftB = collisionRectB.getX();

						rightA = collisionRectA.getX() + collisionRectA.getWidth();
						rightB = collisionRectB.getX() + collisionRectB.getWidth();

						topA = collisionRectA.getY();
						topB = collisionRectB.getY();

						bottomA = collisionRectA.getY() + collisionRectA.getHeight();
						bottomB = collisionRectB.getY() + collisionRectB.getHeight();

					//	Wenn folgende Bedingungen alle zutreffen, dann ber�hren oder �berlappen sich die Rechtecke - es liegt eine Kollision vor
					if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
					{
						//	Kollision f�r "gA" ausl�sen
						gA->collision();

						//	Festhalten, dass bereits eine Kollision f�r "gA" festgestellt wurde
						collision = true;

							//	Diesen for-loop verlassen
							break;
						}
						//	Wurde bereits eine Kollision festgestellt, so wird dieser for-loop verlassen
						if (collision)
							break;
					}
					//	Wurde bereits eine Kollision festgestellt, so wird dieser for-loop verlassen
					if (collision)
						break;
				}
			}
			//	Wurde bereits eine Kollision festgestellt, so wird dieser for-loop verlassen
			if (collision)
				break;
		}
	}
}
