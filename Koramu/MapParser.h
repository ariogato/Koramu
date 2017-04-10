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

	static bool parse(std::string, DataStructure::Stack<Environment::Map>*, FiniteStateMachine::GameStateID);					//	Parsen initialisieren

private:
};