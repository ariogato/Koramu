#include "Animation.h"
#include "TextureManager.h"

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::load(const ParamLoader& params)
{
	SDL_GameObject::load(params);
}

void Animation::update()
{
	SDL_GameObject::update();
}

void Animation::draw(const Vector2D& layerPosition)
{

	/*	Dem TextureManager werden alle benötigten Daten zum Rendern der
	*	Animation übergeben, damit dieser ein wunderschönes Bild
	*	malen kann.
	*	Die Position der gerenderten Textur ist abhängig von der Position des "ObjectLayer"s.
	*	Die Animation kann dank dieser Methode (im Gegensatz zu einem "SDL_GameObject") auch skaliert werden.
	*/
	TheTextureManager::Instance()->drawScaledFrame(m_textureId,
		m_objectRect.positionVector.getX() + layerPosition.getX(), m_objectRect.positionVector.getY() + layerPosition.getY(),
		m_objectRect.width, m_objectRect.height,
		m_currentRow, m_currentCol, m_numRows, m_numCols);

	//	Im Debugmodus werden 'objectRect' und eventuelles 'collisionRect' mitgerendert
	m_objectRect.draw(layerPosition);
	for (int i = 0; i<m_collisionRects.size(); i++)
	{
		m_collisionRects[i].draw(layerPosition);
	}
}
