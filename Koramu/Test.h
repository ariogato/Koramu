#pragma once

//	Header von zu testenden Klassen m�ssen immer einbezogen werden
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Game.h"
#include "TextureManager.h"
#include "SDL_GameObject.h"
#include "ParamLoader.h"
#include "Stack.h"
#include "PauseState.h"
#include "PlayState.h"
#include "InputHandler.h"
#include "Fps.h"
#include "Player.h"
#include "StateParser.h"

/*	Test-Objekte werden erstellt um eine seperate Klasse zu testen.
*	In dieser Test-Klasse wurde das Enturfsmuster der Singleteon-Klasse benutzt.
*
*	Wenn man eine Klasse testen will, sollte man erstmal ein Referenzattribut zu der jeweiligen
*	Klasse erstellen (Idealerweise als Pointer mit Initialisierung im Konstruktor).
*	Anschlie�end in der Funktion Test::testFunctions arbeiten.
*/

class Test
{
private:
	Test();		//	Konstruktor
	~Test();	//	Destruktor

//	TODO: erkl�ren bzw. kommentieren
#pragma region TestAttributes
	SDL_GameObject* player;
	Mix_Music* backgroundMusic;
	StateParser stateParser;
#pragma endregion

public:
	/*	testFunctions() wird in der Hauptschleife in main.cpp aufgerufen.
	*	Alles was getestet werden soll geh�rt hier rein.
	*/
	void testFunctions();
	
	
	/*	Wichtig f�r Singleton-Klasse
	*	'destroy()' ist der "Ersatz-Desktruktor"
	*/
	static Test* Instance();
	static void destroy();


private:
	//	Notwendig f�r Singleton-Klasse
	static Test* s_pInstance;
};

typedef Test TheTester;