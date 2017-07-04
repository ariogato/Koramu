#include "SDL_GameObject.h"
#include "ParamLoader.h"
#include "TextureManager.h"

SDL_GameObject::SDL_GameObject()
	: m_currentRow(0), m_currentCol(0), m_numCols(0),
	  m_numRows(0), m_animSpeed(0), m_velocity(0.0f, 0.0f)
{
}

SDL_GameObject::~SDL_GameObject()
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

	/*	Attribute des 'objectRect's werden gesetzt
	 *		Standardmäßig wird von jedem Spielobjekt das 'objectRect'
	 *		mit der dazugehörigen Beschreibung im Debugmodus angezeigt.
	 */
	m_objectRect.load(parameters);
	m_objectRect.setShowText(true);
	m_objectRect.setVisible(true);

	/*	Attribute des 'collisionRect's sollen hier ausdrücklich nicht gesetzt werden
	 *		Ob für das Objekt eine Kollisionsbox vorhanden sein soll, 
	 *		wird erst bei den Erben von 'SDL_GameObject' (Player, Button, ...) festgelegt.
	 */
	m_collisionRect.setColor(0, 0, 255, 255);
	m_collisionRect.setShowText(false);
	m_collisionRect.setVisible(false);
}

void SDL_GameObject::update()
{
	//	Der anzuzeigende Frame wird berechnet
	m_currentCol = (int)(SDL_GetTicks() / m_animSpeed) % m_numCols;

	//	Den Geschwindigkeitsvektor zum Ortsvektor addieren
	m_objectRect.positionVector += m_velocity;
	m_collisionRect.positionVector += m_velocity;

	//	Im Debugmodus werden 'objectRect' und 'collisionRect' mitgeupdatet
	m_objectRect.update();
	m_collisionRect.update();
}

void SDL_GameObject::draw()
{
	/*	Dem TextureManager werden alle benötigten Daten zum Rendern des 
	 *	Spielobjekts übergeben, damit dieser ein wunderschönes Bild
	 *	malen kann. (im besten Fall eins vom Spielobjekt ;-) ) 
	 */
	TheTextureManager::Instance()->drawFrame(m_textureId,
		m_objectRect.positionVector.getX(), m_objectRect.positionVector.getY(),
		m_objectRect.width, m_objectRect.height,
		m_currentRow, m_currentCol);

	//	Im Debugmodus werden 'objectRect' und 'collisionRect' mitgerendert
	m_objectRect.draw();
	m_collisionRect.draw();
}

void SDL_GameObject::collision()
{
	/*	Der Geschwindigkeitsvektor wird wieder vom Ortsvektor subtrahiert,
	 *	damit die Bewegung aus 'update()' wieder rückgängig gemacht wird.
	 */
	m_objectRect.positionVector -= m_velocity;
	m_collisionRect.positionVector -= m_velocity;
	m_currentCol = 0; // TODO: wollen wir das?
}

void SDL_GameObject::destroy()
{
	SDL_GameObject::~SDL_GameObject();
}
