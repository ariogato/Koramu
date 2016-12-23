#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include <SDL_image.h>
#include "TextureManager.h"

class Game
{
	/*	Konstruktor und Destruktor wurden aus folgendem Grund 'private' gesetzt: 
	*	
	*	Da wir während des gesamten Programmablaufes nur eine einzige 
	*	Instanz dieser Klasse benötigen (und haben dürfen), 
	*	bietet sich das Design-Pattern einer Singleton-Klasse an.
	*
	*	Wie funktionieren Singletons?
	*		Dadurch dass sowohl Konstruktor als auch Desktuktor 'private' sind,
	*		kann keiner eine Instanz dieser Klasse (direkt) erzeugen (oder löschen).
	*		Um trotzdem auf irgendeine Instanz zugreifen zu können, gibt es die 
	*		Methode 'Instance()', die bei Aufruf die statische (ebenfalls 'private')
	*		Member-Variable 's_pInstance' zurückgibt.
	*	
	*	Wie greife ich auf die Instanz zu?
	*		Dadurch dass ganz unten in dieser Datei noch ein 'typedef' eingebaut wurde, 
	*		greift man auf die Klasse 'Game' jetzt mit 'TheGame' zu.
	*		Also gibt Folgendes die Instanz zurück:
	*			'TheGame::Instance()'
	*
	*	Gruß Ario 	
	*/

private:
	Game();
	~Game();

private:
	//	Notwendig für Singleton-Klasse
	static Game* s_pInstance;

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	bool m_running;
	int m_gameWidth;
	int m_gameHeight;
	int m_gameXPos;
	int m_gameYPos;

public:
	bool init(	std::string title,		//	Das Spiel (und SDL) initialisieren
		int width, int height, 
		int xPos, int yPos, 
		int flags);
	void handleInput();					//	Eingaben bearbeiten
	void update();						//	"Back-End"-Sachen (basierend auf den Eingaben) berechnen
	void render();						//	Alles auf den Bildschirm schmeißen


	//	Einfache getter-Funktionen
	SDL_Window* getWindow() { return m_pWindow; }
	SDL_Renderer* getRenderer() { return m_pRenderer; }
	bool isRunning() { return m_running; }
	int getGameWidth() { return m_gameWidth; }
	int getGameHeight() { return m_gameHeight; }
	int getGameXPos() { return m_gameXPos; }
	int getGameYPos() { return m_gameYPos; }


	/*	Wichtig für Singleton-Klasse
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
*		//Eine Instanz der Klasse 'Foo' könnte man folgendermaßen erzeugen:
*
*		TotallyCoolAndNotExaggeratedlyLongName coolObject();
*/
typedef Game TheGame;