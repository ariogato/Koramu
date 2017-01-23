#include "InputHandler.h"

//	Wichtig für Singleton-Klasse
InputHandler* InputHandler::s_pInstance = nullptr;

InputHandler::InputHandler()
	: m_aKeyboardState(nullptr)
{}

InputHandler::~InputHandler()
{
	/*	Es ist schon sinnvoll, dass hier drinnen nichts steht.
	*	Der Array, auf den "m_aKeyboardState" zeigt ist ein SDL-interner Array
	*	und wird von SDL selber zum Schluss freigegeben.
	*/
}

bool InputHandler::isKeyDown(SDL_Scancode key)
{
	/*	Gibt den Wert der Taste aus dem Array 
	*	als boolean zurück
	*/
	
	//	Zuerst muss gecheckt werden, ob "KeyboardState" schon befüllt wurde
	if (m_aKeyboardState)
	{
		return static_cast<bool>(m_aKeyboardState[key]);
	}

	//	Falls "KeyboardState" noch nicht befüllt wurde, soll false zurückgegeben werden.
	return false;
}

void InputHandler::handleInput()
{
	//	Ein Objekt zum speichern eines Ereignisses (Keyboard-Input, Fenster schließen, etc.)
	SDL_Event e;

	//	Die Schleife wird durchlaufen sobald ein Ereignis verfügbar ist.
	while (SDL_PollEvent(&e))
	{
		/*	In dem folgenden "switch"-Block wird herausgefunden was passiert ist.
		*	Also von welchem Typ das Ereignis ist.
		*/
		switch (e.type)
		{
		//	Beim Schließen des Fensters
		case SDL_QUIT:
			//	Der Gameover-Zustand wird in "Game" gesetzt
			TheGame::Instance()->setGameOver();
			break;

		//	Beim Drücken einer Taste
		case SDL_KEYDOWN:
			this->onKeyDown();
			break;

		//	Beim Loslassen einer Taste
		case SDL_KEYUP:
			this->onKeyUp();
			break;
		}
	}
}

void InputHandler::onKeyDown()
{
	//	Hier wird der Zustand der Tastatur abgerufen
	m_aKeyboardState = SDL_GetKeyboardState(nullptr);
}

void InputHandler::onKeyUp()
{
	//	Hier wird der Zustand der Tastatur abgerufen
	m_aKeyboardState = SDL_GetKeyboardState(nullptr);
}

InputHandler * InputHandler::Instance()
{
	/*	Checken ob die Instanz schon existiert.
	*	wenn nein:
	*		eine neue erstellen
	*/
	if (s_pInstance == nullptr)
		s_pInstance = new InputHandler();

	//	Instanz zurückgeben
	return s_pInstance;
}

void InputHandler::destroy()
{
	//	Den Destruktor aufrufen
	delete s_pInstance;

	//	Nochmal - weil es gute Programmierpraxis ist - die Instanz = 'nullptr' setzen
	s_pInstance = nullptr;
}
