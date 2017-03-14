#pragma once

#include "tinyxml2.h"
#include "TextureManager.h"
#include "Logger.h"
#include "GameStateMachine.h"
#include "MenuState.h"
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

	bool parse(const char*);					//	Parsen initialisieren
	bool loadTextures();						//	Texturen laden
	bool loadGameObjects(XMLNode*);				//	GameObjects laden
	
};
