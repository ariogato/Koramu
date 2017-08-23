#pragma once

#include <vector>
#include <map>
#include "GameObject.h"

namespace Environment
{
	class Map;
}

namespace DataStructure
{
	template<typename T>
	class Stack;
}

namespace FiniteStateMachine
{
	enum GameStateID;
}


/*	Diese Klasse hat den Zweck die Maps aus der "maps.xml" Datei 
 *	unserere mit "Tiled" erstellten Maps (gespeichert in tmx-Dateien) auszulesen.
 *
 *	Methoden geben bei Erfolg "true", andernfalls "false" zurück.
 *	
 *	Die Methoden wurden als 'static' deklariert, um auch ohne Instanziierung eines Objektes auf
 *	sie zugreifen zu können.
 */

class MapParser
{
public:
	MapParser();			//	Konstruktor
	~MapParser();			//	Destruktor

	static bool parse(	std::string filename, 
						std::map<std::string, Environment::Map*>& pMapDict, DataStructure::Stack<Environment::Map>& pMapStack, 
						std::vector<GameObject*>* pObjects, FiniteStateMachine::GameStateID stateID);								//	Parsen initialisieren

private:
	static bool parseMap(std::string path, Environment::Map* pMap);																	//	Bestimmte Map parsen
};