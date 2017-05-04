#include "MapParser.h"
#include "tinyxml2.h"
#include "Map.h"
#include "Stack.h"
#include "Game.h"
#include "GameState.h"
#include "ParamLoader.h"

using namespace tinyxml2;

MapParser::MapParser()
{
}

MapParser::~MapParser()
{
}

bool MapParser::parse(std::string filename, std::map<std::string, Environment::Map*>& pMapDict, DataStructure::Stack<Environment::Map*>& pMapStack, std::vector<GameObject*>* pObjects, FiniteStateMachine::GameStateID stateID)
{
#pragma region Document
	//	Ein neues Dokument wird erstellt und ge�ffnet
	XMLDocument* pDocument = new XMLDocument();
	
	//	Laden des Dokuments (anhand des �bergebenen Dateinamens).
	if (pDocument->LoadFile(filename.c_str()))
	{
		/*	Beim Laden ist ein Fehler passiert.
		*	Diesen Fehler loggen wir.
		*/
		TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << " konnte nicht geladen werden. " << pDocument->ErrorName() << std::endl << std::endl;

		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
		return false;
	}
#pragma endregion 
	
#pragma region StateRoot
	//	Ermitteln des Wurzelelementes
	XMLElement* pStateRoot = pDocument->RootElement();
	if (!pStateRoot)
	{
		/*	Die XML-Datei besitzt kein Wurzelelement und ist demnach leer.
		*	Diesen Fehler loggen wir.
		*/
		TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << " hat kein <states>-Element." << std::endl << std::endl;

		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
		return false;
	}
#pragma endregion 

#pragma region StateNode
	//	XMLElement des angeforderten Spielzustandes ermitteln
	XMLElement* pStateNode = pStateRoot->FirstChildElement(FiniteStateMachine::stateNames[stateID]);

	//	Checken, ob 'pStateNode' bef�llt wurde
	if (!pStateNode)
	{
		/*	Der Spielzustand besitzt kein <states>-Element und ist demnach leer.
		*	Diesen Fehler loggen wir.
		*/
		TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << " hat keinen \"" << FiniteStateMachine::stateNames[stateID] << "\" Zustand." << std::endl << std::endl;

		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
		return false;
	}
#pragma endregion 

#pragma region MapNode
	//	XMLElement der Maps ermitteln (<maps> ist immer das 1. Element)
	XMLElement* pMapNode = pStateNode->FirstChildElement();

	//	Checken, ob 'pMapNode' bef�llt wurde
	if (!pMapNode)
	{
		/*	Der Spielzustand besitzt kein <maps>-Element und ist demnach leer.
		*	Diesen Fehler loggen wir.
		*/
		TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << " hat kein <maps>-Element im Zustand \"" << FiniteStateMachine::stateNames[stateID] << "\"." << std::endl << std::endl;
	
		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
		return false;
	}

	//	Checken, ob <maps> bef�llt ist
	if (pMapNode->NoChildren())
	{
		//	Lies die Log-Message du Vogel!
		TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << " hat keine Maps im Zustand \"" << FiniteStateMachine::stateNames[stateID] << "\"." << std::endl << std::endl;
		
		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
		return false;
	}
#pragma endregion 

	//	Hilfsvariable f�r die Fehlerausgabe
	int counter = 1;

	/*	Hier iterieren wir �ber die einzelnen Maps in <maps>
	 *	und parsen jede Map, bevor wir uns an die n�chste machen. 
	 */
	for (XMLElement* e = pMapNode->FirstChildElement(); e != nullptr; e = e->NextSiblingElement(), counter++)
	{
		//	Daten der Map werden ausgelesen
		std::string mapId = e->Attribute("id");
		std::string mapPath = e->Attribute("path");

#pragma region Validation
		//	Die Daten werden auf Validit�t gepr�ft
		if (mapId.empty())
		{
			TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << ": Die " << counter << ". Map im Zustand \"" << FiniteStateMachine::stateNames[stateID] << "\" hat keine id." << std::endl << std::endl;

			//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
			return false;
		}
		if (mapPath.empty())
		{
			TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << ": Die " << counter << ". Map im Zustand \"" << FiniteStateMachine::stateNames[stateID] << "\" hat keinen path." << std::endl << std::endl;

			//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
			return false;
		}
#pragma endregion 

		//	Hier wird die Map aus 'parseMap()' hineingeparst
		//	TODO: Funktion checken (immer neues Objekt oder nicht?)
		Environment::Map* pCurrentMap = new Environment::Map();

		/*	Aufrufen der Methode zum Laden der Map und �berpr�fen, ob das Laden erfolgreich war.
		 *	Die geparste Map wird in 'pCurrentMap' geladen.
		 */
		if (!parseMap(mapPath, pCurrentMap))
		{
			TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << ": Die " << counter << ". Map im Zustand \"" << FiniteStateMachine::stateNames[stateID] << "\" konnte nicht geparst werden." << std::endl << std::endl;
		
			//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
			return false;
		}
	}

	//	Pointer l�schen, um einem Speicherleck vorzubeugen.
	delete pDocument;

	//	Das Parsen ist reibungsfrei abgelaufen. Wir geben "true" zur�ck.
	return true;
}

bool MapParser::parseMap(std::string path, Environment::Map* map)
{
#pragma region Document
	//	'.tmx'-Datei wird geladen
	XMLDocument* pMapDocument = new XMLDocument();
	//	Laden des Dokuments (anhand des �bergebenen Dateinamens).
	if (pMapDocument->LoadFile(path.c_str()))
	{
		TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t" << path << " konnte nicht geladen werden. " << pMapDocument->ErrorName() << std::endl << std::endl;

		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
		return false;
	}
#pragma endregion

#pragma region RootElement
	//	Ermitteln des Wurzelelementes
	XMLElement* pMapRoot = pMapDocument->RootElement();
	if (!pMapRoot)
	{
		TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t" << path << " hat kein Wurzelelement." << std::endl << std::endl;

		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
		return false;
	}
#pragma endregion

	ParamLoader params;
#pragma region MapData
	//	Variablen zur Speicherung von Mapdaten
	int width, height;
	int tilewidth, tileheight;

	//	width
	if (pMapRoot->QueryAttribute("width", &width))
	{
		TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path <<": Das <map>-Element besitzt keine width" << std::endl << std::endl;
		return false;
	}
	//	height
	if (pMapRoot->QueryAttribute("height", &height))
	{
		TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <map>-Element besitzt keine height" << std::endl << std::endl;
		return false;
	}
	//	tilewidth
	if (pMapRoot->QueryAttribute("tilewidth", &tilewidth))
	{
		TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <map>-Element besitzt keine tilewidth" << std::endl << std::endl;
		return false;
	}
	//	tileheight
	if (pMapRoot->QueryAttribute("tileheight", &tileheight))
	{
		TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <map>-Element besitzt keine tileheight" << std::endl << std::endl;
		return false;
	}

	params.setWidth(width);
	params.setHeight(height);
#pragma endregion

	//	Pointer l�schen, um einem Speicherleck vorzubeugen.
	delete pMapDocument;

	//	Das Parsen ist reibungsfrei abgelaufen. Wir geben "true" zur�ck.
	return true;
}
