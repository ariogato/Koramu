#pragma once

#include <string>
#include <vector>
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


struct Tileset
{
	int firstGid;
	int tilecount;
};

class MapParser
{
public:
	MapParser();			//	Konstruktor
	~MapParser();			//	Destruktor

	static bool parse(std::string filename, DataStructure::Stack<Environment::Map>& pMapStack, std::vector<GameObject*>* pObjects, FiniteStateMachine::GameStateID stateID);					//	Parsen initialisieren

private:
};