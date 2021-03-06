#pragma once

#include "tinyxml2.h"
#include "GameState.h"
#include <vector>

class GameObject;

using namespace tinyxml2;

/*	Diese Klasse hat den Zweck Spielzust�nde aus der "states.xml" Datei auszulesen.
*	
*	Methoden geben bei Erfolg "true", andernfalls "false" zur�ck.
*
*	Der jeweilige State ruft die parse() Funktion einer Instanz des Parsers auf,
*	sobald der State ganz oben im State-Stapel ist und seine onEnter() Funktion aufgerufen wird.
*	Die parse() Funktion �bergibt der State einen Pointer auf seine Liste aus GameObjects.
*
*	Die Methoden wurden als 'static' deklariert, um auch ohne Instanziierung eines Objektes auf
*	sie zugreifen zu k�nnen.
*/

class StateParser
{
private:
	static bool m_sHasLoadedTextures;						//	Zeigt an, ob die Texturen bereits geladen wurden
public:
	StateParser();			//	Konstruktor
	~StateParser();			//	Destruktor

	static bool parse(std::string filename, std::vector<GameObject*>* pObjects, FiniteStateMachine::GameStateID stateID);				//	Parsen initialisieren

private:
	static bool loadTextures(XMLElement* pTextureNode);																					//	Texturen laden
	static bool loadGameObjects(XMLElement* pCurrentStateNode, std::vector<GameObject*>* pObjects);										//	GameObjects laden
};
