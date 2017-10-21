#include "SDL_GameObject.h"
#include "ParamLoader.h"
#include "TextureManager.h"
#include "ScriptManager.h"


SDL_GameObject::SDL_GameObject()
	: m_currentRow(0), m_currentCol(0), m_numCols(1),
	  m_numRows(1), m_animSpeed(1), m_velocity(0.0f, 0.0f)
{
}

SDL_GameObject::~SDL_GameObject()
{
}

void SDL_GameObject::loadScript()
{
}

void SDL_GameObject::load(const ParamLoader& parameters)
{
	/*	Die Werte von m_currentRow und m_currentCol ändern sich erst bei den Erben von 
	*	SDL_GameObject (Player, Enemy, ...), 
	*	denn es werden kaum Objekte der Klasse SDL_GameObject existieren und wenn sie existieren, 
	*	werden sie wahrscheinlich nur einen Frame als Textur haben.
	*/
	m_currentRow = 0;
	m_currentCol = 0;

	//	Ab hier werden alle übergebenen Werte geladen
	m_numCols = parameters.getNumCols();
	m_numRows = parameters.getNumRows();
	m_animSpeed = parameters.getAnimSpeed();
	m_textureId = parameters.getTextureId();
	m_mapId = parameters.getMapId();
	m_uniqueId = parameters.getUniqueId();

	/*	Attribute des 'objectRect's werden gesetzt
	 *		Standardmäßig wird von jedem Spielobjekt das 'objectRect'
	 *		mit der dazugehörigen Beschreibung im Debugmodus angezeigt.
	 */
	m_objectRect.load(parameters);
	m_objectRect.setShowText(true);
	m_objectRect.setVisible(true);	
}

void SDL_GameObject::update()
{
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
	/*	Dem TextureManager werden alle benötigten Daten zum Rendern des 
	 *	Spielobjekts übergeben, damit dieser ein wunderschönes Bild
	 *	malen kann. (im besten Fall eins vom Spielobjekt ;-) ) 
	 *	Die Position der gerenderten Textur ist abhängig von der Position des "ObjectLayer"s.
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
	/*	Der Geschwindigkeitsvektor wird wieder vom Ortsvektor subtrahiert,
	 *	damit die Bewegung aus 'update()' wieder rückgängig gemacht wird.
	 */
	m_objectRect.positionVector -= m_velocity;
	for (int i = 0; i<m_collisionRects.size(); i++)
	{
		m_collisionRects[i].positionVector -= m_velocity;
	}

	//	Die Animation des Spielobjekts wird auf Stillstand mit Blick in die selbe Richtung gesetzt
	m_currentCol = 0;

	//	Die Funktion im Script aufrufen
	TheScriptManager::Instance()->getScriptFromId(m_uniqueId).callFunction("onCollision");
}

void SDL_GameObject::onCreate()
{
	//	Die Funktion im Script aufrufen
	TheScriptManager::Instance()->getScriptFromId(m_uniqueId).callFunction("onCreate");
}

void SDL_GameObject::interact(Player* pPlayer)
{
}

void SDL_GameObject::destroy()
{
	SDL_GameObject::~SDL_GameObject();
}
