#include "MapParser.h"
#include "tinyxml2.h"
#include "Map.h"
#include "Stack.h"
#include "Game.h"
#include "GameState.h"

using namespace tinyxml2;

MapParser::MapParser()
{
}

MapParser::~MapParser()
{
}

bool MapParser::parse(std::string filename, DataStructure::Stack<Environment::Map>& pMapStack, std::vector<GameObject*>* pObjects, FiniteStateMachine::GameStateID stateID)
{
	//	Ein neues Dokument wird erstellt und ge�ffnet
	XMLDocument* pDocument = new XMLDocument();
	pDocument->LoadFile(filename.c_str());

	//	Ermitteln des Wurzelelementes
	XMLElement* pStateRoot = pDocument->RootElement();
	if (pStateRoot == nullptr)
	{
		/*	Die XML-Datei besitzt kein Wurzelelement und ist demnach leer.
		*	Diesen Fehler loggen wir.
		*/
		TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << " hat kein <states>-Element." << std::endl << std::endl;

		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
		return false;
	}

	XMLElement* pStateNode = pStateRoot->FirstChildElement(FiniteStateMachine::stateNames[stateID]);

	//	Pointer l�schen, um einem Speicherleck vorzubeugen.
	delete pDocument;

	//	Das Parsen ist reibungsfrei abgelaufen. Wir geben "true" zur�ck.
	return true;
}
