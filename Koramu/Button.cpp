#include "Button.h"
#include "ParamLoader.h"
#include "InputHandler.h"
#include "Vector2D.h"
#include "Game.h"
#include "TextureManager.h"

Button::Button()
	: m_callbackFunction(nullptr),
	m_fixCol(0)
{
}


Button::~Button()
{
}

void Button::load(const ParamLoader& params)
{
	SDL_GameObject::load(params);
	m_callbackId = params.getCallbackId();
}

void Button::update()
{
	SDL_GameObject::update();

	//	Die Maus-Position speichern
	Vector2D mousePosition(TheInputHandler::Instance()->getMousePosition());

	//	m_currentCol wird gesetzt (standardm��ig auf 0)
	m_currentCol = m_fixCol;

	//	Ist "m_fixCol" nicht 0, so soll das Bild des Buttons nichtmehr ge�ndert werden. Auch bei einem Klick soll nichts passieren.
	//	Checken ob die xPosition der Maus in dem Bereich des Buttons ist
	if (mousePosition.getX() >= m_objectRect.positionVector.getX() && mousePosition.getX() <= m_objectRect.positionVector.getX() + m_objectRect.width && !m_fixCol)
	{
		//	Checken ob die yPosition der Maus in dem Bereich des Buttons ist
		if (mousePosition.getY() >= m_objectRect.positionVector.getY() && mousePosition.getY() <= m_objectRect.positionVector.getY() + m_objectRect.height)
		{
			m_currentCol = 1;
			
			//	Checken ob die linke Maustaste bet�tigt wurde
			if (TheInputHandler::Instance()->getLeftMouseButtonState())
			{
				m_currentCol = 2;
				//	Es wird gewartet, bis die Taste losgelassen wird
				while (TheInputHandler::Instance()->getLeftMouseButtonState())
				{
					/*	W�hrend die Maustaste gedr�ckt bleibt, muss weiterhin der Input verarbeitet werden.
					 *	Des Weiteren muss auch gerendert werden, damit man den Effekt des Button-Clicks sieht.
					 */
					TheGame::Instance()->handleInput();
					TheGame::Instance()->render();
					SDL_Delay(20);
				}

				//	Hier wird gecheckt, ob die Maus sich nach dem Loslassen der Taste noch auf dem Button befindet.
				mousePosition = TheInputHandler::Instance()->getMousePosition();

				if (mousePosition.getX() >= m_objectRect.positionVector.getX() && mousePosition.getX() <= m_objectRect.positionVector.getX() + m_objectRect.width)
					if (mousePosition.getY() >= m_objectRect.positionVector.getY() && mousePosition.getY() <= m_objectRect.positionVector.getY() + m_objectRect.height)
						//	Zuletzt wird die Callback Funktion aufgerufen.
						m_callbackFunction();
			}
		}
	}
}

void Button::draw(const Vector2D& layerPosition)
{
	/*	Dem TextureManager werden alle ben�tigten Daten zum Rendern des
	*	Buttons �bergeben, damit dieser ein wundersch�nes Bild
	*	malen kann.
	*	Die Position der gerenderten Textur ist abh�ngig von der Position des "ObjectLayer"s.
	*	Der Button kann dank dieser Methode (im Gegensatz zu einem "SDL_GameObject") auch skaliert werden. 
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
