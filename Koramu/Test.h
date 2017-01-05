#pragma once

//	Header von zu testenden Klassen m�ssen immer einbezogen werden
#include "Game.h"
#include "TextureManager.h"
#include "SDL_GameObject.h"
#include "ParamLoader.h"


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

#pragma region TestAttributes
	SDL_GameObject* player;
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