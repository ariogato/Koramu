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
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
}


bool Game::init(std::string title, int width, int height, int xPos, int yPos, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
	{
		//	Die Initialisierung von SDL ist fehlgeschlagen! Fehlermeldung ausgeben und false zur�ckgeben:
		std::cerr << "SDL_Init failed: \n" << SDL_GetError() << std::endl;
		return false;
	}
	else 
	{
		//	Die initialiesierung von SDL war erfolgreich!
		std::cout << "SDL wurde erfolgreich intiialisiert!" << std::endl;


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


		//	m_running = true; vorerst auskommentiert lassen (sonst l�uft das Programm in einer Endlosschleife)


		//	Wenn wir hier angekommen sind ist nichts schief gelaufen
		return true;
		
	}
	
}


void Game::handleInput()
{

}

void Game::update() 
{

}

void Game::render()
{

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
	//	Den Destruktor aufrufen
	delete s_pInstance;

	//	Nochmal - weil es gute Programmierpraxis ist - die Instanz = 'nullptr' setzen
	s_pInstance = nullptr;
}