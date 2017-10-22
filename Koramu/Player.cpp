#include "Player.h"
#include <vector>
#include "ParamLoader.h"
#include "InputHandler.h"
#include "Game.h"
#include "CommandQueue.h"

Player::Player()
{}

Player::~Player()
{}

void Player::load(const ParamLoader& params)
{
	//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
	SDL_GameObject::load(params);


	/*	Hier wird die Kollisionsbox des 'Player's definiert.
	 *	Die offset Variablen beschreiben die Position der Kollisionsbox
	 *	innerhalb des 'Player'-Objekts in Abhängigkeit von der oberen linken Ecke.
	 */
	ParamLoader collisionParams;
	float collisionOffsetX, collisionOffsetY;
	int collisionWidth, collisionHeight;

	collisionWidth = 60;
	collisionHeight = 30;
	collisionOffsetX = 5;	
	collisionOffsetY = params.getHeight() - collisionHeight;

	collisionParams.setX(params.getX() + collisionOffsetX);
	collisionParams.setY(params.getY() + collisionOffsetY);
	collisionParams.setWidth(collisionWidth);
	collisionParams.setHeight(collisionHeight);

	ObjectRectangle tempRectangle;
	tempRectangle.load(collisionParams);
	tempRectangle.setColor(0, 0, 255, 0);
	m_collisionRects.push_back(tempRectangle);

	/*	Die Beschreibung der Box soll nicht angzeigt werden, 
	 *	während die Box selber durchaus angezeigt werden soll
	 */
	m_collisionRects[0].setVisible(true);
	m_collisionRects[0].setShowText(false);

	/*	Hier werden die Interaktionsboxen des 'Player's definiert.
	 *	Es läuft ähnlich wie bei den Kollisionsboxen
	 */

	for (int i = 0; i < 4; i++)
	{
		//	Die Variablen für die Rechtecke deklarieren
		ParamLoader interactParams;
		int interactOffsetX, interactOffsetY, interactWidth, interactHeight;


		/*	Zwei der Rechtecke sollen jeweils die gleiche Größe haben (oben & unten, links & rechts).
		 *	Des Weiteren sind unterscheiden sich die Paare nur in x-Position bzw. y-Position
		 *	
		 *	Die Position wird in Abhängigkeit von der des CollisionRects angegeben
		 */
		if (i % 2 == 0)
		{
			//	Werte, die gleich sind setzen
			interactWidth = collisionWidth;
			interactHeight = 20;
			interactOffsetX = 0;

			//	Die y-Position von oberem und unterem Rechteck setzen
			interactOffsetY = (i == 0) ? (-1 * interactHeight) : (collisionHeight);
		}
		else
		{
			//	Werte, die gleich sind setzen
			interactWidth = 20;
			interactHeight = collisionHeight;
			interactOffsetY = 0;

			//	Die y-Position von oberem und unterem Rechteck setzen
			interactOffsetX = (i == 1) ? (collisionWidth) : (-1 * interactWidth);
		}
		
		//	Werte werden gesetzt
		interactParams.setWidth(interactWidth);
		interactParams.setHeight(interactHeight);
		interactParams.setX(m_collisionRects[0].getX() + interactOffsetX);
		interactParams.setY(m_collisionRects[0].getY() + interactOffsetY);

		ObjectRectangle interactRect;
		interactRect.load(interactParams);
		interactRect.setColor(0, 255, 0, 255);
		interactRect.setVisible(true);
		interactRect.setShowText(false);
		m_interactRects.push_back(interactRect);
	}

	//	Die Skripts laden (Attribute, die im Skript gesetzt wurden evtl übernehmen)
	loadScript();
}

GameObject* Player::interactCollision()
{
	/*	Zuerst muss das passende 'interactRect' des 'Player' Objektes ausgewählt werden.
	 *	Dies geschieht abhängig von der Blickrichtung des Objektes, also ganz einfach und ziemlich unschön:
	 *	'm_currentRow'.
	 *	
	 *	0 : nach unten -> m_interactRects[2]
	 *	1 : nach oben -> m_interactRects[0]
	 *	2 : nach rechts -> m_interactRects[1]
	 *	3 : nach links -> m_interactRects[3]
	 *	
	 *	Anschließend wird geprüft, ob dieses Rechteck mit irgendeinem Objekt kollidiert und falls ja, wird 
	 *	dieses Objekt zurückgegeben.
	 */

	//	Das richtige 'interactRect' basierend auf 'm_currentRow' heraussuchen
	int interactRectNumber;
	switch (m_currentRow)
	{
	case 0:
		interactRectNumber = 2;
		break;
	case 1:
		interactRectNumber = 0;
		break;
	case 2:
		interactRectNumber = 1;
		break;
	case 3:
		interactRectNumber = 3;
		break;
	default:
		interactRectNumber = 0;
	}
	ObjectRectangle& interactRect = m_interactRects[interactRectNumber];

	//	Variablen für die Kollisionserkennung deklarieren
	int leftA, rightA, topA, bottomA,
		leftB, rightB, topB, bottomB;

	for (auto pGameObject : *(TheGame::Instance()->getCurrentState()->getCurrentMap()->getObjectLayer()->getGameObjects()))
	{
		//	Eine Kollision mit sich selbst macht keinen Sinn
		if (this == pGameObject)
			continue;

		//	Über die Kollisionsrechtecke des Spielobjekts "gB" iterieren
		for (auto collisionRect : pGameObject->getCollisionRects())
		{
			//	Die Formel zur Kollisionserkennung; Copyright Roman
			leftA = interactRect.getX();
			leftB = collisionRect.getX();

			rightA = interactRect.getX() + interactRect.getWidth();
			rightB = collisionRect.getX() + collisionRect.getWidth();

			topA = interactRect.getY();
			topB = collisionRect.getY();

			bottomA = interactRect.getY() + interactRect.getHeight();
			bottomB = collisionRect.getY() + collisionRect.getHeight();

			//	Wenn folgende Bedingungen alle zutreffen, dann berühren oder überlappen sich die Rechtecke - es liegt eine Kollision vor
			if (rightA >= leftB && bottomA >= topB && topA <= bottomB && leftA <= rightB)
			{
				return pGameObject;
			}
		}
	}

	//	Falls keine Kollision vorliegt
	return nullptr;
}

void Player::loadScript()
{
}

void Player::update()
{
	m_currentCol = 0;
	m_velocity.setX(0.0f);
	m_velocity.setY(0.0f);

	//	Falls ein Befehl vorliegt, soll der Input nicht entgegengenommen werden
	if (m_pCommands->isEmpty())
	{
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT))
		{
			Vector2D temp(2.0f, 0.0f);

			m_velocity += temp;
		}
		else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT))
		{
			Vector2D temp(-2.0f, 0.0f);

			m_velocity += temp;
		}
		else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP))
		{
			Vector2D temp(0.0f, -2.0f);

			m_velocity += temp;
		}
		else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN))
		{
			Vector2D temp(0.0f, 2.0f);

			m_velocity += temp;
		}

		//	Checken, ob ein Objekt zum interagieren vohanden ist (nicht löschen; gehört ObjectLayer)
		GameObject* pInteractionObject = interactCollision();
		if (pInteractionObject)
		{
			//	Checken, ob die Interaktionstaste bedient wurde
			if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
			{
				//	Warten, bis die Taste losgelassen wurde
				while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
				{
					//	Diese Lösung ist nicht ansatzweise elegant, aber es funktioniert
					TheInputHandler::Instance()->handleInput();
					TheGame::Instance()->render();
					SDL_Delay(20);
				}

				TheGame::Instance()->logStandard() << "Interaktion mit: " << pInteractionObject->getUniqueId() << std::endl;

				//	Die Interaktionsmethode des Objektes aufrufen
				pInteractionObject->interact(this);
			}
		}
	}
	else
	{
		//	Den aktuellen Befehl ausführen
		m_pCommands->update();
	}

	//	Falls sich der Spieler bewegt, updaten
	if (m_velocity.getLength())
	{
		//	currentRow abhängig von der Bewegungsrichtung setzen

		//	Bewegung in x-Richtung
		if (m_velocity.getX() != 0)
		{
			//	Bewegung in positive x-Richtung
			if (m_velocity.getX() > 0)
				m_currentRow = 2;

			//	Bewegung in negative x-Richtung
			if (m_velocity.getX() < 0)
				m_currentRow = 3;
		}
		//	Bewegung in y-Richtung
		else
		{
			//	Bewegung in positive y-Richtung
			if (m_velocity.getY() > 0)
				m_currentRow = 0;

			//	Bewegung in negative y-Richtung
			if (m_velocity.getY() < 0)
				m_currentRow = 1;
		}

		//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
		SDL_GameObject::update();

		//	Die Position der interactRectangles updaten
		for (int i = 0; i < m_interactRects.size(); i++)
		{
			m_interactRects[i].positionVector += m_velocity; 
			m_interactRects[i].update();
		}
	}
}

void Player::draw(const Vector2D& layerPosition)
{
	//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
	SDL_GameObject::draw(layerPosition);

	//	InteractRectangles im Debugmodus mitrendern
	for (int i = 0; i < m_interactRects.size(); i++)
		m_interactRects[i].draw(layerPosition);
}

void Player::collision()
{
	SDL_GameObject::collision();

	//	Die interactRects auch kollidieren lassen
	for (int i = 0; i < m_interactRects.size(); i++)
		m_interactRects[i].positionVector -= m_velocity;
}

void Player::onCreate()
{
	SDL_GameObject::onCreate();
}
