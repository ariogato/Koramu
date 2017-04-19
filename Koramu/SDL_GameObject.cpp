#include "SDL_GameObject.h"
#include "ParamLoader.h"
#include "TextureManager.h"

SDL_GameObject::SDL_GameObject()
	: m_positionVector(0.0f, 0.0f)
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
	m_positionVector.setX(parameters.getX());
	m_positionVector.setY(parameters.getY());
	m_width = parameters.getWidth();
	m_height = parameters.getHeight();
	m_numCols = parameters.getNumCols();
	m_numRows = parameters.getNumRows();
	m_animSpeed = parameters.getAnimSpeed();
	m_textureId = parameters.getTextureId();
	m_mapId = parameters.getMapId();
}

void SDL_GameObject::update()
{
	//	Der anzuzeigende Frame wird berechnet
	m_currentCol = (int)(SDL_GetTicks() / m_animSpeed) % m_numCols;
}

void SDL_GameObject::draw()
{
	TheTextureManager::Instance()->drawFrame(m_textureId,
		m_positionVector.getX(), m_positionVector.getY(),
		m_width, m_height,
		m_currentRow, m_currentCol);
}

void SDL_GameObject::destroy()
{
	SDL_GameObject::~SDL_GameObject();
}
