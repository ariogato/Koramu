#include "InputHandler.h"
#include "Game.h"

//	Wichtig f�r Singleton-Klasse
InputHandler* InputHandler::s_pInstance = nullptr;

InputHandler::InputHandler()
	:	m_aKeyboardState(nullptr), 
		m_pMousePosition(new Vector2D(0.0f, 0.0f))
{
	//	Der Array f�r Maustastenzust�nde wird initialisiert, wobei jeder Wert "false" gesetzt wird
	for (int i = 0; i < 3; i++)
		m_mouseButtonStates.push_back(false);
}

InputHandler::~InputHandler()
{
	/*	Es ist schon sinnvoll, dass "m_aKeyboardState" hier drinnen nicht steht.
	*	Der Array, auf den "m_aKeyboardState" zeigt ist ein SDL-internes Array
	*	und wird von SDL selber zum Schluss freigegeben.
	*/
	delete m_pMousePosition;
}

void InputHandler::handleInput()
{
	//	Ein Objekt zum speichern eines Ereignisses (Keyboard-Input, Fenster schlie�en, etc.)
	SDL_Event event;

	//	Die Schleife wird durchlaufen sobald ein Ereignis verf�gbar ist.
	while (SDL_PollEvent(&event))
	{
		/*	In dem folgenden "switch"-Block wird herausgefunden was passiert ist.
		*	Also von welchem Typ das Ereignis ist.
		*/
		switch (event.type)
		{
		//	Beim Schlie�en des Fensters
		case SDL_QUIT:
			//	Der "Gameover-Zustand" wird in "Game" gesetzt
			TheGame::Instance()->setGameOver();
			break;

		//	Beim Dr�cken einer Taste
		case SDL_KEYDOWN:
			this->onKeyDown();
			break;

		//	Beim Loslassen einer Taste
		case SDL_KEYUP:
			this->onKeyUp();
			break;

		//	Beim Bewegen der Maus
		case SDL_MOUSEMOTION:
			this->onMouseMotion(event);
			break;

		//	Beim Dr�cken einer Maustaste
		case SDL_MOUSEBUTTONDOWN:
			this->onMouseButtonDown(event);
			break;

		//	Beim Loslassen einer Maustaste
		case SDL_MOUSEBUTTONUP:
			this->onMouseButtonUp(event);
			break;
		}
	}
}

void InputHandler::onKeyDown()
{
	//	Hier wird der Zustand der Tastatur abgerufen
	m_aKeyboardState = SDL_GetKeyboardState(nullptr);

	//	Restliche 'SDL_KEYDOWN' events werden gel�scht
	SDL_FlushEvent(SDL_KEYDOWN);
}

void InputHandler::onKeyUp()
{
	//	Hier wird der Zustand der Tastatur abgerufen
	m_aKeyboardState = SDL_GetKeyboardState(nullptr);
	
	//	Restliche 'SDL_KEYUP' events werden gel�scht
	SDL_FlushEvent(SDL_KEYUP);
}

void InputHandler::onMouseMotion(const SDL_Event& event)
{
	//	Hier wird anhand des Events die Position des Mauszeigers ermittelt
	m_pMousePosition->setX(event.motion.x); 
	m_pMousePosition->setY(event.motion.y);

	//	Restliche 'SDL_MOUSEMOTION' events werden gel�scht
	SDL_FlushEvent(SDL_MOUSEMOTION);
}

void InputHandler::onMouseButtonDown(const SDL_Event& event)
{
	/*	Zuerst muss gepr�ft werden welche Maustaste gedr�ckt wurde.
	*	Anschlie�end wird der Zustand der Taste (gedr�ckt) in Form eines
	*	Wahrheitswertes (true) in "m_mouseButtonStates" gespeichert.
	*
	*	Hierbei wird jeder Taste auf der Maus ein Index in dem Array zugeordnet:
	*		0 - Links
	*		1 - Mitte
	*		2 - Rechts
	*/

	//	Pr�fen, welche Taste gedr�ckt wurde
	switch (event.button.button)
	{
	//	Beim Dr�cken der linken Maustaste
	case SDL_BUTTON_LEFT:
		m_mouseButtonStates[0] = true;
		break;

	//	Beim Dr�cken der mittleren Maustaste (Scrollrad)
	case SDL_BUTTON_MIDDLE:
		m_mouseButtonStates[1] = true;
		break;

	//	Beim Dr�cken der rechten Maustaste 
	case SDL_BUTTON_RIGHT:
		m_mouseButtonStates[2] = true;
		break;
	}

	//	Restliche 'SDL_MOUSEBUTTONDOWN' events werden gel�scht
	SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
}

void InputHandler::onMouseButtonUp(const SDL_Event& event)
{
	//	Details: siehe "onMouseButtonDown"

	//	Pr�fen, welche Taste lsogelassen wurde
	switch (event.button.button)
	{
	//	Beim Loslassen der linken Maustaste
	case SDL_BUTTON_LEFT:
		m_mouseButtonStates[0] = false;
		break;

	//	Beim Loslassen der mittleren Maustaste (Scrollrad)
	case SDL_BUTTON_MIDDLE:
		m_mouseButtonStates[1] = false;
		break;

	//	Beim Loslassen der rechten Maustaste 
	case SDL_BUTTON_RIGHT:
		m_mouseButtonStates[2] = false;
		break;
	}

	//	Restliche 'SDL_MOUSEBUTTONUP' events werden gel�scht
	SDL_FlushEvent(SDL_MOUSEBUTTONUP);
}

bool InputHandler::isKeyDown(SDL_Scancode key)
{
	/*	Gibt den Wert der Taste aus dem Array
	*	als boolean zur�ck
	*/

	//	Zuerst muss gecheckt werden, ob "KeyboardState" schon bef�llt wurde
	if (m_aKeyboardState)
	{
		return static_cast<bool>(m_aKeyboardState[key]);
	}

	//	Falls "KeyboardState" noch nicht bef�llt wurde, soll false zur�ckgegeben werden.
	return false;
}

Vector2D InputHandler::getMousePosition() const
{
	return *m_pMousePosition;
}

InputHandler* InputHandler::Instance()
{
	/*	Checken ob die Instanz schon existiert.
	*	wenn nein:
	*		eine neue erstellen
	*/
	if (s_pInstance == nullptr)
		s_pInstance = new InputHandler();

	//	Instanz zur�ckgeben
	return s_pInstance;
}

void InputHandler::destroy()
{
	//	Den Destruktor aufrufen
	delete s_pInstance;

	//	Nochmal - weil es gute Programmierpraxis ist - die Instanz = 'nullptr' setzen
	s_pInstance = nullptr;
}
