#include "Button.h"
#include "ParamLoader.h"
#include "InputHandler.h"
#include "Vector2D.h"
#include "Game.h"
#include "PlayState.h"
#include "GameStateMachine.h"
#include "Stack.h"

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
			if (TheInputHandler::Instance()->getLeftMouseButtonState())
			{
				m_currentCol = 2;
				m_callbackFunction();
				std::cout << "asddfsa";
			}
		}
	}
}

void Button::draw()
{
	SDL_GameObject::draw();
}
