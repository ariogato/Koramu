#pragma once

//	Header von zu testenden Klassen müssen immer einbezogen werden
#include "Game.h"
#include "Vector2D.h"


/*	Test-Objekte werden erstellt um eine seperate Klasse zu testen.
*	In dieser Test-Klasse wurde das Enturfsmuster der Singleteon-Klasse benutzt.
*
*	Wenn man eine Klasse testen will, sollte man erstmal ein Referenzattribut zu der jeweiligen
*	Klasse erstellen (Idealerweise als Pointer mit Initialisierung im Konstruktor).
*	Anschließend in der Funktion Test::testFunctions arbeiten.
*/

class Test
{
private:
	Test();		//	Konstruktor
	~Test();	//	Destruktor

#pragma region TestAttribute
	Vector2D* v;
#pragma endregion

public:
	/*	testFunctions() wird in der Hauptschleife in main.cpp aufgerufen.
	*	Alles was getestet werden soll gehört hier rein.
	*/
	void testFunctions();
	
	
	/*	Wichtig für Singleton-Klasse
	*	'destroy()' ist der "Ersatz-Desktruktor"
	*/
	static Test* Instance();
	static void destroy();


private:
	//	Notwendig für Singleton-Klasse
	static Test* s_pInstance;
};

typedef Test TheTester;