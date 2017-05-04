#pragma once

#include <string>
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


class MapParser
{
public:
	MapParser();			//	Konstruktor
	~MapParser();			//	Destruktor

	static bool parse(	std::string filename, 
						std::map<std::string, Environment::Map*>& pMapDict, DataStructure::Stack<Environment::Map*>& pMapStack, 
						std::vector<GameObject*>* pObjects, FiniteStateMachine::GameStateID stateID);								//	Parsen initialisieren

private:
	static bool parseMap(std::string path, Environment::Map* map);
};