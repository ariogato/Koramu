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
	//	Wenn wir hier angekommen sind ist nichts schief gelaufen
	return true;
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