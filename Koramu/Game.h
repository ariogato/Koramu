#pragma once

#include <string>
#include <SDL.h>
#include "Logger.h"
#include "GameState.h"

namespace FiniteStateMachine
{
	class GameStateMachine;
}

/*	Konstruktor und Destruktor wurden aus folgendem Grund 'private' gesetzt:
*
*	Da wir w�hrend des gesamten Programmablaufes nur eine einzige
*	Instanz dieser Klasse ben�tigen (und haben d�rfen),
*	bietet sich das Design-Pattern einer Singleton-Klasse an.
*
*	Wie funktionieren Singletons?
*		Dadurch dass sowohl Konstruktor als auch Desktuktor 'private' sind,
*		kann keiner eine Instanz dieser Klasse (direkt) erzeugen (oder l�schen).
*		Um trotzdem auf irgendeine Instanz zugreifen zu k�nnen, gibt es die
*		Methode 'Instance()', die bei Aufruf die statische (ebenfalls 'private')
*		Member-Variable 's_pInstance' zur�ckgibt.
*
*	Wie greife ich auf die Instanz zu?
*		Dadurch dass ganz unten in dieser Datei noch ein 'typedef' eingebaut wurde,
*		greift man auf die Klasse 'Game' jetzt mit 'TheGame' zu.
*		Also gibt Folgendes die Instanz zur�ck:
*			'TheGame::Instance()'
*
*	Zustandsmaschine:
*		Wir implementieren unser Spiel auf Basis einer FSM (Finite State Machine).
*		Das hei�t, dass unser Spiel zwischen verschiedenen Zust�nden (z.B. Pause, Men�) unterscheiden kann 
*		und in der Folge auch die zustandsspezifischen Operationen durchf�hren wird. 
*		Hierzu muss unsere grundlegende Klasse Game eine Membervariable besitzen, die unsere ganzen Zust�nde verwaltet ('m_pStateMachine').
*		
*		Zu 'm_pCurrentState':
*			Sobald eine Zustands�nderung stattfinden sollte, wird der neue Zustand in 'm_pCurrentState' lediglich zwischengespeichert.
*			Das Rendern (und andere Operationen) des alten Zustands wird zuendegef�hrt.
*			Erst beim n�chsten Durchlauf der Hauptschleife (siehe main.cpp) tritt der neue Zustand tats�chlich ein.
*/

class Game
{

private:
	Game();			//	Konstruktor
	~Game();		//	Destruktor

private:
	//	Notwendig f�r Singleton-Klasse
	static Game* s_pInstance;

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	
	//	Zustandsmaschine
	FiniteStateMachine::GameStateMachine* m_pStateMachine;

	//	Logger Variablen
	Logger* m_pStandardLog;
	Logger* m_pErrorLog;

	bool m_running;
	int m_gameWidth;
	int m_gameHeight;
	int m_gameXPos;
	int m_gameYPos;

	FiniteStateMachine::GameState* m_pCurrentState;			//	Aktueller Spielzustand

	//	Variablen f�r die Zustandsmaschine
	bool isChangeState = false;
	bool isPushState = false;
	bool isPopState = false;

public:
	bool init(	std::string title,		//	Das Spiel (und SDL) initialisieren
		int width, int height, 
		int xPos, int yPos, 
		int flags);
	void handleInput();							//	Eingaben bearbeiten
	void update();								//	"Back-End"-Sachen (basierend auf den Eingaben) berechnen
	void render();								//	Alles auf den Bildschirm schmei�en
	void setGameOver();							//	Um z.B. vom InputHandler aus das Spiel zu beenden
	void emergencyExit(const char* message);	//	Um, im Fall eines fatalen Fehlers, das Programm sofort zu beenden (nicht empfohlen!)

	//	Methoden zur Erfassung einer Zustands�nderung
	void changeState(FiniteStateMachine::GameState* newState);
	void pushState(FiniteStateMachine::GameState* newStates);
	void popState();

	//	Einfache getter-Funktionen
	SDL_Window* getWindow() { return m_pWindow; }
	SDL_Renderer* getRenderer() { return m_pRenderer; }
	bool isRunning() const { return m_running; }
	int getGameWidth() const { return m_gameWidth; }
	int getGameHeight() const { return m_gameHeight; }
	int getGameXPos() const { return m_gameXPos; }
	int getGameYPos() const { return m_gameYPos; }

	/*	Log Objekte werden zur�ckgegeben, damit man mit einem
	*	Stream Operator auf sie zugreifen kann.
	*/
	Logger& logStandard() { return *m_pStandardLog; }
	Logger& logError() { return *m_pErrorLog; }


	/*	Wichtig f�r Singleton-Klasse
	*	'destroy()' ist der "Ersatz-Desktruktor"
	*/
	static Game* Instance();
	static void destroy();
};

/*	Ein typedef ist nur dazu da um einer Klasse einen anderen 
*	(evtl. cooleren Namen zuzuweisen).
*
*	Bsp.:
*		class Foo {};
*		typedef Foo TotallyCoolAndNotExaggeratedlyLongName;
*
*		//Eine Instanz der Klasse 'Foo' k�nnte man folgenderma�en erzeugen:
*
*		TotallyCoolAndNotExaggeratedlyLongName coolObject();
*/
typedef Game TheGame;