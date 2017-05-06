#include "Game.h"
#include "TextureManager.h"
#include "Test.h"
#include "GameStateMachine.h"
#include "InputHandler.h"
#include <iostream>
#include <SDL_image.h>
#include "GameObjectFactory.h"
#include "Player.h"
#include "MenuState.h"

/*	Wichtig für Singleton-Klasse
*	
*	side fact:	'nullptr' ist das Pendant zu Java's 'null'.
*				Also "nichts". Man könnte auch 'NULL' oder '0' stattdessen
*				benutzen - hätte den selben Effekt - man sollte trotzdem immmer
*				'nullptr' benutzen.
*
*	Gruß Ario
*/
Game* Game::s_pInstance = nullptr;

Game::Game()									//	Konstruktor
	:	m_running(false), 
		m_gameWidth(0), m_gameHeight(0), 
		m_gameXPos(0), m_gameYPos(0)
{
	//	Die Logger initialisieren
	m_pStandardLog = new Logger();
	m_pErrorLog = new Logger("../logs/errors.txt");

	// Pointer mit nullptr initialisieren (best practice)
	m_pWindow = nullptr;
	m_pRenderer = nullptr;
	m_pStateMachine = nullptr;
}

/*	!! WICHTIG !!
*	
*	Sobald man mindestens ein dynamisch alloziiertes Objekt 
*	(Schema: "pointer = new class()") als Member-Variable hat 
*	MUSS MUSS MUSS man einen Destruktor schreiben,
*	der diese löscht. Sonst entsteht ein übles Speicherleck!!!
*
*	(Hier wäre das eher unwichtig, weil das Programm sowieso endet, nachdem 
*	 die Instanz dieser Klasse gelöscht wurde)
*
*	Gruß Ario
*/

Game::~Game()									//	Destruktor
{
	//	Dynamisch allozierte Objeke löschen
	delete m_pStandardLog;
	delete m_pErrorLog;
	delete m_pStateMachine;

	SDL_DestroyRenderer(m_pRenderer);			//	Den Renderer zerstören
	SDL_DestroyWindow(m_pWindow);				//	Das Fenster zerstören
	IMG_Quit();									//	SDL_image beenden
	SDL_Quit();									//	SDL beenden
}


bool Game::init(std::string title, int width, int height, int xPos, int yPos, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
	{
		//	Die Initialisierung von SDL ist fehlgeschlagen! Fehlermeldung ausgeben und false zurückgeben.
		std::cerr << "SDL_Init failed: \n" << SDL_GetError() << std::endl;
		return false;
	}
	else if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{ 
		/*	Das Zeug in der obigen Zeile bedeutet nur,
		*	dass man checkt, ob das, was man initialisieren wollte auch wirklich
		*	erfolgreich initialisiert wurde.
		*/

		//	Falls wir hier ankommen, ist etwas kaputt gegangen
		std::cerr << "IMG_Init failed: \n" << IMG_GetError() << std::endl;
		return false;
	}
	else 
	{
		//	Die initialisierung von SDL & SDL_image war erfolgreich!
		std::cout <<	"SDL wurde erfolgreich initialisiert!" << std::endl <<
						"SDL_image wurde erfolgreich initialisiert!" << std::endl;


		//	Fenster erstellen. Es werden Parameter, die Game::init mitgegeben worden sind an SDL_CreateWindow() weitergegeben:
		m_pWindow = SDL_CreateWindow(title.c_str(), xPos, yPos, width, height, flags);

		//	Überprüfen, ob das Fenster erfolgreich erstellt wurde:
		if (!m_pWindow) 
		{
			//	Die Erstellung des Fensters ist fehlgeschlagen! Fehlermeldung ausgeben und false zurückgeben:
			std::cerr << "Could not create window: \n" << SDL_GetError() << std::endl;
			return false;
		}
		//	Das Fenster wurde erfolgreich erstellt
		std::cout << "Fenster wurde erfolgreich erstellt!" << std::endl;


		//	Renderer erstellen.
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

		//	Überprüfen, ob der Renderer erfolgreich erstellt wurde:
		if (!m_pRenderer)
		{
			//	Die Erstellung des Renderers ist fehlgeschlagen! Fehlermeldung ausgeben und false zurückgeben:
			std::cerr << "Could not create renderer: \n" << SDL_GetError() << std::endl;
			return false;
		}
		//	Der Renderer wurde erfolgreich erstellt
		std::cout << "Renderer wurde erfolgreich erstellt!" << std::endl;

		//	Zustandsmaschine initialisieren
		m_pStateMachine = new FiniteStateMachine::GameStateMachine();

		//	Das lässt die main-Schleife laufen
		m_running = true;

		//	Informationen über das Fenster speichern
		m_gameWidth = width;
		m_gameHeight = height;
		m_gameXPos = xPos;
		m_gameYPos = yPos;

#pragma region GameObjectFactoryTest
		TheGameObjectFactory::Instance()->registerType("button", new PlayerCreator());
		TheGameObjectFactory::Instance()->registerType("animation", new PlayerCreator());
#pragma endregion

		//	Zu Beginn des Spiels wird der 'MenuState' aufgerufen
		m_pStateMachine->pushState(new FiniteStateMachine::MenuState());

		//	Wenn wir hier angekommen sind ist nichts schief gelaufen
		return true;
		
	}
	
}


void Game::handleInput()
{
	/*	Wir haben eine eigene Klasse, die den Input für uns händelt.
	*	Folglich wird hier nur die "handleInput"-Methode dieser aufgerufen.
	*
	*	Diese Methode prüft auch, ob das Fenster geschlossen werden soll. (X gedrückt)
	*/
	TheInputHandler::Instance()->handleInput();
}

void Game::update() 
{

}

void Game::render()
{
	/*	Das was bis jetzt im Renderer ist soll 
	*	rausgehauen werden.
	*	Sonst würde man jeden Frame immer wieder sehen. 
	*/
	SDL_RenderClear(m_pRenderer);

	/*	Hier wird die 'GameStateMachine' dazu aufgefordert den aktuellen 
	 *	Spielzustand zu rendern.
	 *	Welcher Spielzustand gerade gerendert werden soll 
	 *	interessiert die Klasse 'Game' nicht
	 */
	m_pStateMachine->render();

#pragma region testStuff
	TheTester::Instance()->testFunctions();
#pragma endregion

	//	Jetzt wird alles auf den Bildschirm geschmissen
	SDL_RenderPresent(m_pRenderer);
}

void Game::setGameOver()
{
	m_running = false;
}


//	Wichtig für Singleton-Klasse
Game* Game::Instance()
{
	/*	Checken ob die Instanz schon existiert.
	*	wenn nein:
	*		eine neue erstellen
	*/
	if (s_pInstance == nullptr)
		s_pInstance = new Game();

	//	Instanz zurückgeben
	return s_pInstance;
}

//	Wichtig für Singleton-Klasse
void Game::destroy()
{
	//	Den TextureManager bei Beendigung des Spiels zerstören
	TheTextureManager::Instance()->destroy();

	//	Den InputHandler bei Beendigung des Spiels zerstören
	TheInputHandler::Instance()->destroy();

	//	Den Destruktor aufrufen
	delete s_pInstance;

	//	Nochmal - weil es gute Programmierpraxis ist - die Instanz = 'nullptr' setzen
	s_pInstance = nullptr;
}