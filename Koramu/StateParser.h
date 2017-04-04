#pragma once

#include "tinyxml2.h"
#include "TextureManager.h"
#include "Logger.h"
#include "GameStateMachine.h"
#include "MenuState.h"
//	#include "GameState.h"
#include <iostream>

/*	Diese Klasse hat den Zweck Spielzustände aus der "states.xml" Datei auszulesen.
*	Methoden geben bei Erfolg "true", andernfalls "false" zurück.
*/

using namespace tinyxml2;

class StateParser
{
private:
	XMLDocument* m_pDocument;

public:
	StateParser();			//	Konstruktor
	~StateParser();			//	Destruktor

	bool parse(std::string, FiniteStateMachine::GameStateID);					//	Parsen initialisieren
	bool loadTextures(XMLElement*);												//	Texturen laden
	bool loadGameObjects(XMLElement*);											//	GameObjects laden
};
