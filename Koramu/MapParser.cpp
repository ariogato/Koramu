#include "MapParser.h"
#include "tinyxml2.h"

using namespace tinyxml2;

MapParser::MapParser()
{
}

MapParser::~MapParser()
{
}

bool MapParser::parse(std::string filename, DataStructure::Stack<Environment::Map>* pMapStack, FiniteStateMachine::GameStateID stateID)
{
	//	Ein neues Dokument wird erstellt und geöffnet
	XMLDocument* pDocument = new XMLDocument();
	pDocument->LoadFile(filename.c_str());


}
