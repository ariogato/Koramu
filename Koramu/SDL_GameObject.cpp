#include "SDL_GameObject.h"
#include "ParamLoader.h"
#include "TextureManager.h"
#include "ScriptManager.h"
#include "CommandQueue.h"
#include "MoveCommand.h"
#include "StunCommand.h"
#include "Game.h"


SDL_GameObject::SDL_GameObject()
	: m_currentRow(0), m_currentCol(0), m_numCols(1),
	  m_numRows(1), m_animSpeed(1), m_velocity(0.0f, 0.0f),
	  m_pCommands(new CommandQueue(*this))
{
}

SDL_GameObject::~SDL_GameObject()
{
	delete m_pCommands;
}

void SDL_GameObject::loadScript()
{
}

void SDL_GameObject::load(const ParamLoader& parameters)
{
	/*	Die Werte von m_currentRow und m_currentCol �ndern sich erst bei den Erben von 
	*	SDL_GameObject (Player, Enemy, ...), 
	*	denn es werden kaum Objekte der Klasse SDL_GameObject existieren und wenn sie existieren, 
	*	werden sie wahrscheinlich nur einen Frame als Textur haben.
	*/
	m_currentRow = 0;
	m_currentCol = 0;

	//	Ab hier werden alle �bergebenen Werte geladen
	m_numCols = parameters.getNumCols();
	m_numRows = parameters.getNumRows();
	m_animSpeed = parameters.getAnimSpeed();
	m_textureId = parameters.getTextureId();
	m_mapId = parameters.getMapId();
	m_uniqueId = parameters.getUniqueId();

	/*	Attribute des 'objectRect's werden gesetzt
	 *		Standardm��ig wird von jedem Spielobjekt das 'objectRect'
	 *		mit der dazugeh�rigen Beschreibung im Debugmodus angezeigt.
	 */
	m_objectRect.load(parameters);
	m_objectRect.setShowText(true);
	m_objectRect.setVisible(true);	
}

void SDL_GameObject::update()
{
	//	Die Zeile wird basierend auf dem Geschwindigkeitsvektor errechnet
	if (m_velocity.getLength() > 0)
	{
		//	Bewegung nach unten
		if (m_velocity.getY() > 0)
		{

			if (m_numRows > 0)
				m_currentRow = 0;
		}
		//	Bewegung nach oben
		else if (m_velocity.getY() < 0)
		{

			if (m_numRows > 1)
				m_currentRow = 1;

		}
		//	Bewegung nach rechts
		else if (m_velocity.getX() > 0) 
		{
			if (m_numRows > 2)
				m_currentRow = 2;
		}

		//	Bewegung nach links
		else
		{
			if (m_numRows > 3)
				m_currentRow = 3;
		}
	
	}

	//	Der anzuzeigende Frame wird berechnet
	m_currentCol = (int)(SDL_GetTicks() / m_animSpeed) % m_numCols;

	//	Den Geschwindigkeitsvektor zum Ortsvektor addieren
	m_objectRect.positionVector += m_velocity;
	for (int i = 0; i<m_collisionRects.size(); i++)
	{
		m_collisionRects[i].positionVector += m_velocity;
	}
		
	//	Im Debugmodus werden 'objectRect' und 'collisionRects' mitgeupdatet
	for(int i = 0; i<m_collisionRects.size(); i++)
	{
		m_collisionRects[i].update();
	}

	m_objectRect.update();
}

void SDL_GameObject::draw(const Vector2D& layerPosition)
{
	/*	Dem TextureManager werden alle ben�tigten Daten zum Rendern des 
	 *	Spielobjekts �bergeben, damit dieser ein wundersch�nes Bild
	 *	malen kann. (im besten Fall eins vom Spielobjekt ;-) ) 
	 *	Die Position der gerenderten Textur ist abh�ngig von der Position des "ObjectLayer"s.
	 */
	TheTextureManager::Instance()->drawFrame(m_textureId,
		m_objectRect.positionVector.getX() + layerPosition.getX(), m_objectRect.positionVector.getY() + layerPosition.getY(),
		m_objectRect.width, m_objectRect.height,
		m_currentRow, m_currentCol);

	//	Im Debugmodus werden 'objectRect' und 'collisionRect' mitgerendert
	m_objectRect.draw(layerPosition);
	for (int i = 0; i<m_collisionRects.size(); i++)
	{
		m_collisionRects[i].draw(layerPosition);
	}
}

void SDL_GameObject::collision()
{
	//	Falls gerade ein Move Befehl vorliegt, soll dieser gel�scht werden, da bei einer Kollision Schluss ist
	if (!m_pCommands->isEmpty())
		if (m_pCommands->getCurrentCommand()->getType() == COMMAND_MOVE)
			m_pCommands->getCurrentCommand()->setDone();

	/*	Der Geschwindigkeitsvektor wird wieder vom Ortsvektor subtrahiert,
	 *	damit die Bewegung aus 'update()' wieder r�ckg�ngig gemacht wird.
	 */
	m_objectRect.positionVector -= m_velocity;
	for (int i = 0; i<m_collisionRects.size(); i++)
	{
		m_collisionRects[i].positionVector -= m_velocity;
	}

	//	Die Animation des Spielobjekts wird auf Stillstand mit Blick in die selbe Richtung gesetzt
	m_currentCol = 0;

	//	Die Funktion im Script aufrufen, wenn sich das Objekt auf der selben Map wie der Spieler befindet
 	if (!TheGame::Instance()->getCurrentState()->getCurrentMapId().compare(this->getMapId()) || this->getMapId().empty())
		TheScriptManager::Instance()->getScriptById(m_uniqueId).callFunction("onCollision");
}

void SDL_GameObject::onCreate()
{
	//	Die Funktion im Script aufrufen
	TheScriptManager::Instance()->getScriptById(m_uniqueId).callFunction("onCreate");
}

void SDL_GameObject::interact(Player* pPlayer)
{
}

void SDL_GameObject::moveToPosition(Vector2D v)
{
	m_pCommands->pushCommand(new MoveCommand(v));
}

void SDL_GameObject::moveRelative(Vector2D v)
{
	m_pCommands->pushCommand(new MoveCommand(getPosition() + v));
}

void SDL_GameObject::stun(float sec)
{
	m_pCommands->pushCommand(new StunCommand(sec));
}

void SDL_GameObject::pushCommand(BaseCommand* pCommand)
{
	//	Checken, ob nullptr �bergeben wurde
	if (!pCommand)
		return;
	
	//	Falls nicht, soll der Befehl gepusht werden
	m_pCommands->pushCommand(pCommand);
}

void SDL_GameObject::popCommand()
{
	//	Checken, ob die Liste leer ist
	if (m_pCommands->isEmpty())
		return;

	//	Falls nicht, soll das erste Element entfernt werden
	m_pCommands->popCommand();
}

void SDL_GameObject::onCommandDone(const char* commandType)
{
	//	Checken, ob nullptr �bergeben wurde
	if (!commandType)
		return;

	//	Das Skript wird zwischengespeichert
	Script s =
		TheScriptManager::Instance()->getScriptById(m_uniqueId);

	//	self wird gepusht (das muss einfach so sein)
	s.pushTable();

	//	Den Typen des Befehls als string Argument auf den Stack pushen
	s.pushArgumentString(commandType);

	//	Die Funktion des Scripts aufrufen
	s.callVoidWithArgs("onCommandDone", 2);
}

void SDL_GameObject::destroy()
{
	SDL_GameObject::~SDL_GameObject();
}

BaseCommand* SDL_GameObject::getCurrentCommand() const
{
	//	Falls die Liste leer ist, soll nullptr zur�ckgegeben werden
	if (m_pCommands->isEmpty())
		return nullptr;

	//	Andernfalls soll das Element am Anfang der Liste zur�ckgegeben werden
	return m_pCommands->getCurrentCommand();
}

void SDL_GameObject::setPosition(float x, float y)
{
	//	Ermitteln, um wie weit das Objekt und eine Kollisionsrechtecke zu bewegen sind
	Vector2D moveVector(x, y);
	moveVector -= m_objectRect.positionVector;
	
	//	Object(Rectangle) mithilfe des "moveVector"s auf die gew�nschte Position setzen
	m_objectRect.positionVector += moveVector;
	//	Aktualisieren, damit der angezeigte Text von Anfang an stimmt
	m_objectRect.update();

	//	�ber die Kollsionsrechtecke des Objekts iterieren
	for(auto &cRect : m_collisionRects)
	{
		//	Aktuelles Kollisionsrechteck an die gew�nschte Position verschieben
		cRect.positionVector += moveVector;
		cRect.update();
	}
}
