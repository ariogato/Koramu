#pragma once

#include "tinyxml2.h"
#include "GameState.h"
#include <vector>

class GameObject;

/*	Diese Klasse hat den Zweck Spielzustände aus der "states.xml" Datei auszulesen.
*	Methoden geben bei Erfolg "true", andernfalls "false" zurück.
*	
*	Der jeweilige State ruft die parse() Funktion einer Instanz des Parsers auf,
*	sobald der State ganz oben im State-Stapel ist und seine onEnter() Funktion aufgerufen wird.
*	Die parse() Funktion übergibt der State einen Pointer auf seine Liste aus GameObjects 
*	(die muss er irgendwie von den Layern bekommen).
*	
*	Die Methoden wurden als 'static' deklariert, um auch ohne Instanziierung eines Objektes auf
*	sie zugreifen zu können.
*/

using namespace tinyxml2;

class StateParser
{
private:
	static bool m_sHasLoadedTextures;						//	Zeigt an, ob die Texturen bereits geladen wurden
public:
	StateParser();			//	Konstruktor
	~StateParser();			//	Destruktor

	static bool parse(std::string, std::vector<GameObject*>*, FiniteStateMachine::GameStateID);					//	Parsen initialisieren
	static bool loadTextures(XMLElement*);																		//	Texturen laden
	static bool loadGameObjects(XMLElement*, std::vector<GameObject*>*);										//	GameObjects laden
};
