#include <iostream>
#include <SDL.h>
#include "Game.h"

#define DEBUG 0

#if DEBUG
#include "Test.h"
#endif

/*	Die Präprozessor-Makros 
*	(so nennt man die "Variablen" (oder "Funktionen") mit #define)
*	sind nur zum Debuggen da. Sie werden später evtl. ersetzt
*/

//	(64x10) : (64x8)
#define WINDOW_WIDTH 1280	
#define WINDOW_HEIGHT 896

//	für die smoothness 
const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char** argv)
{
	/*	Um die Zeit zu messen, 
	*	die für einen Durchlauf benötigt wurde
	*	
	*	Uint32 (weil SDL_GetTicks diesen Datentypen zurückgibt)
	*	ist einfach ein 32-bit Integer
	*/
	Uint32 startTime, frameTime;

	/*	Das Programm/Spiel wird initialisiert
	*	Gleichzeitig wird geprüft ob alles gut lief
	*/
	if ((TheGame::Instance()->init("Koramu", 
		WINDOW_WIDTH, WINDOW_HEIGHT, 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		0)))
	{
		/*	Solange das Spiel noch läuft...
		*	Der Gameover-Zustand wird dies ändern können
		*/
		while (TheGame::Instance()->isRunning())
		{
			//	Anzahl der Ticks am Anfang wird gespeichert (für den übernächsten Schritt)
			startTime = SDL_GetTicks();

			/*	Immer nacheinander den Input auswerten, 
			*	dann "Back-End"-Sachen laufen lassen
			*	und schließlich alles auf den Schirm schmeißen
			*/
			TheGame::Instance()->handleInput();			
			TheGame::Instance()->update();
			TheGame::Instance()->render();

#if DEBUG
			//	Ein Aufruf aller Funktionen, die zu testen sind
			TheTester::Instance()->testFunctions();
#endif 

			/*	Die Zeit, die nun gewartet werden muss, damit eine konstante 60-FPS performance
			*	entsteht, wird jetzt ausgerechnet und 'SDL_Delay()' übergeben.
			*	(vorrausgesetzt der obige Prozess hat nicht zu lang gedauert)
			*/
			frameTime = SDL_GetTicks() - startTime;

			if (frameTime < DELAY_TIME)
				SDL_Delay((int)(DELAY_TIME - frameTime));
		}
		//	Sobald das Spiel nicht mehr läuft wird aufgeräumt
		TheGame::Instance()->destroy();
	}

	std::cout << "ByeBye" << std::endl;

#if DEBUG
	Test::Instance()->destroy();
#endif

	// Damit man lesen kann, was ausgegeben wird und das Fenster nicht sofort verschwindet.
	system("PAUSE"); 
	return 0;
}
