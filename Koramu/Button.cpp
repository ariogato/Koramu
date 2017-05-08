#include "Button.h"
#include "ParamLoader.h"
#include "InputHandler.h"
#include "Vector2D.h"
#include "Game.h"

Button::Button()
	: m_callbackFunction(nullptr)
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

	//	m_currentCol wird standardmäßig auf 0 gesetzt
	m_currentCol = 0;

	//	Checken ob die xPosition der Maus in dem Bereich des Buttons ist
	if (mousePosition.getX() >= m_positionVector.getX() && mousePosition.getX() <=m_positionVector.getX() + m_width)
	{
		//	Checken ob die yPosition der Maus in dem Bereich des Buttons ist
		if (mousePosition.getY() >= m_positionVector.getY() && mousePosition.getY() <= m_positionVector.getY() + m_height)
		{
			m_currentCol = 1;
			
			//	Checken ob die linke Maustaste betätigt wurde
			if (TheInputHandler::Instance()->getLeftMouseButtonState())
			{
				m_currentCol = 2;
				//	Es wird gewartet, bis die Taste losgelassen wird
				while (TheInputHandler::Instance()->getLeftMouseButtonState())
				{
					/*	Während die Maustaste gedrückt bleibt, muss weiterhin der Input verarbeitet werden.
					 *	Des Weiteren muss auch gerendert werden, damit man den Effekt des Button-Clicks sieht.
					 */
					TheGame::Instance()->handleInput();
					TheGame::Instance()->render();
				}

				//	Hier wird gecheckt, ob die Maus sich nach dem Loslassen der Taste noch auf dem Button befindet.
				mousePosition = TheInputHandler::Instance()->getMousePosition();

				if (mousePosition.getX() >= m_positionVector.getX() && mousePosition.getX() <= m_positionVector.getX() + m_width)
					if (mousePosition.getY() >= m_positionVector.getY() && mousePosition.getY() <= m_positionVector.getY() + m_height)
						//	Zuletzt wird die Callback Funktion aufgerufen.
						m_callbackFunction();
			}
		}
	}
}

void Button::draw()
{
	SDL_GameObject::draw();
}
