#include "Game.h"

/*	Wichtig f�r Singleton-Klasse
*	
*	side fact:	'nullptr' ist das Pendant zu Java's 'null'.
*				Also "nichts". Man k�nnte auch 'NULL' oder '0' stattdessen
*				benutzen - h�tte den selben Effekt - man sollte trotzdem immmer
*				'nullptr' benutzen.
*
*	Gru� Ario
*/
Game* Game::s_pInstance = nullptr;

Game::Game()								//	Konstruktor
{
	//	Die Logger initialisieren
	m_pStandardLog = new Logger();
	m_pErrorLog = new Logger("../logs/errors.txt");

	// Pointer mit nullptr initialisieren (best practice)
	m_pWindow = nullptr;
	m_pRenderer = nullptr;
}

/*	!! WICHTIG !!
*	
*	Sobald man mindestens ein dynamisch alloziiertes Objekt 
*	(Schema: "pointer = new class()") als Member-Variable hat 
*	MUSS MUSS MUSS man einen Destruktor schreiben,
*	der diese l�scht. Sonst entsteht ein �bles Speicherleck!!!
*
*	(Hier w�re das eher unwichtig, weil das Programm sowieso endet, nachdem 
*	 die Instanz dieser Klasse gel�scht wurde)
*
*	Gru� Ario
*/
Game::~Game()								//	Destruktor
{
	//	Dynamisch allozierte Objeke l�schen
	delete m_pStandardLog;
	delete m_pErrorLog;

	SDL_DestroyRenderer(m_pRenderer);			//	Den Renderer zerst�ren
	SDL_DestroyWindow(m_pWindow);				//	Das Fenster zerst�ren
	IMG_Quit();									//	SDL_image beenden
	SDL_Quit();									//	SDL beenden
}


bool Game::init(std::string title, int width, int height, int xPos, int yPos, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
	{
		//	Die Initialisierung von SDL ist fehlgeschlagen! Fehlermeldung ausgeben und false zur�ckgeben.
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

		//	�berpr�fen, ob das Fenster erfolgreich erstellt wurde:
		if (!m_pWindow) 
		{
			//	Die Erstellung des Fensters ist fehlgeschlagen! Fehlermeldung ausgeben und false zur�ckgeben:
			std::cerr << "Could not create window: \n" << SDL_GetError() << std::endl;
			return false;
		}
		//	Das Fenster wurde erfolgreich erstellt
		std::cout << "Fenster wurde erfolgreich erstellt!" << std::endl;


		//	Renderer erstellen.
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

		//	�berpr�fen, ob der Renderer erfolgreich erstellt wurde:
		if (!m_pRenderer)
		{
			//	Die Erstellung des Renderers ist fehlgeschlagen! Fehlermeldung ausgeben und false zur�ckgeben:
			std::cerr << "Could not create renderer: \n" << SDL_GetError() << std::endl;
			return false;
		}
		//	Der Renderer wurde erfolgreich erstellt
		std::cout << "Renderer wurde erfolgreich erstellt!" << std::endl;

		//	Zustandsmaschine initialisieren
		m_pStateMachine = new FiniteStateMachine::GameStateMachine();

		//	Das l�sst die main-Schleife laufen
		m_running = true;


		//	Wenn wir hier angekommen sind ist nichts schief gelaufen
		return true;
		
	}
	
}


void Game::handleInput()
{
	/*	Eine provisorische Implementation, die es
	*	erm�glicht das Fenster zu schlie�en.
	*	Das Ganze muss sp�ter in die InputHandler Klasse verschoben werden
	*/

	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			m_running = false;
		}
	}
}

void Game::update() 
{

}

void Game::render()
{
	/*	Das was bis jetzt im Renderer ist soll 
	*	rausgehauen werden.
	*	Sonst w�rde man jeden Frame immer wieder sehen. 
	*/
	SDL_RenderClear(m_pRenderer);

#pragma region testStuff
	TheTester::Instance()->testFunctions();
#pragma endregion

	//	Jetzt wird alles auf den Bildschirm geschmissen
	SDL_RenderPresent(m_pRenderer);
}


//	Wichtig f�r Singleton-Klasse
Game* Game::Instance()
{
	/*	Checken ob die Instanz schon existiert.
	*	wenn nein:
	*		eine neue erstellen
	*/
	if (s_pInstance == nullptr)
		s_pInstance = new Game();

	//	Instanz zur�ckgeben
	return s_pInstance;
}

//	Wichtig f�r Singleton-Klasse
void Game::destroy()
{
	//	Den TextureManager bei Beendigung des Spiels zerst�ren
	TheTextureManager::Instance()->destroy();

	//	Den Destruktor aufrufen
	delete s_pInstance;

	//	Nochmal - weil es gute Programmierpraxis ist - die Instanz = 'nullptr' setzen
	s_pInstance = nullptr;
}