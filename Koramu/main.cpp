#include <iostream>
#include <SDL.h>
#include "Game.h"

/*	Die Pr�prozessor-Makros 
*	(so nennt man die "Variablen" (oder "Funktionen") mit #define)
*	sind nur zum Debuggen da. Sie werden sp�ter evtl. ersetzt
*/

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//	f�r die smoothness 
const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char** argv)
{
	/*	Um die Zeit zu messen, 
	*	die f�r einen Durchlauf ben�tigt wurde
	*	
	*	Uin32 - weil SDL_GetTicks diesen Datentypen zur�ckgibt
	*		  - ist einfach ein 32-bit Integer
	*/
	Uint32 startTime, frameTime;

	/*	Das Programm/Spiel wird initialisiert
	*	Gleichzeitig wird gepr�ft ob alles gut lief
	*/
	if ((TheGame::Instance()->init("Koramu", 
		WINDOW_WIDTH, WINDOW_HEIGHT, 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		0)))
	{
		/*	Solange das Spiel noch l�uft...
		*	Der Gameover-Zustand wird dies �ndern k�nnen
		*/
		while (TheGame::Instance()->isRunning())
		{
			//	Anzahl der Ticks am Anfang wird gespeichert (f�r den �bern�chsten Schritt)
			startTime = SDL_GetTicks();

			/*	Immer nacheinander den Input auswerten, 
			*	dann "Back-End"-Sachen laufen lassen
			*	und schlie�lich alles auf den Schirm schmei�en
			*/
			TheGame::Instance()->handleInput();			
			TheGame::Instance()->update();
			TheGame::Instance()->render();

			/*	Die Zeit, die nun gewartet werden muss, damit eine konstante 60-FPS performance
			*	entsteht, wird nun ausgerechnet und 'SDL_Delay()' �bergeben.
			*	(vorrausgesetzt der obige Prozess hat nicht zu lang gedauert)
			*/
			frameTime = SDL_GetTicks() - startTime;

			if (frameTime < DELAY_TIME)
				SDL_Delay((int)(DELAY_TIME - frameTime));
		}

		//	Sobald das Spiel nicht mehr l�uft wird aufger�umt
		TheGame::Instance()->destroy();
	}

	std::cout << "ByeBye" << std::endl;
	SDL_Delay(1000);
	return 0;
}