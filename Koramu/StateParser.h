#pragma once

#include "tinyxml2.h"
#include "GameState.h"

/*	Diese Klasse hat den Zweck Spielzustände aus der "states.xml" Datei auszulesen.
*	Methoden geben bei Erfolg "true", andernfalls "false" zurück.
*	
*	Der jeweilige State ruft die parse() Funktion einer Instanz des Parsers auf,
*	sobald der State ganz oben im State-Stapel ist und seine onEnter() Funktion aufgerufen wird.
*	Der parse() Funktion übergibt der State einen Pointer auf seine Liste aus GameObjects 
*	(die muss er irgendwie von den Layern bekommen).
*/

using namespace tinyxml2;

class StateParser
{
private:
	XMLDocument* m_pDocument;

public:
	StateParser();			//	Konstruktor
	~StateParser();			//	Destruktor

	bool parse(std::string, FiniteStateMachine::GameStateID);				//	Parsen initialisieren
	bool loadTextures(XMLElement*);											//	Texturen laden
	bool loadGameObjects(XMLElement*);										//	GameObjects laden
};
