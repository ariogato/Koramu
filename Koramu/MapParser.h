#pragma once

#include <string>
#include <vector>
#include "Map.h"

class Map;
class Stack;

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

	static bool parse(std::string filename, DataStructure::Stack<Environment::Map>* pMapStack, FiniteStateMachine::GameStateID stateID);					//	Parsen initialisieren

private:
};