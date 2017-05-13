#include "MapParser.h"
#include <vector>
#include <string>
#include <sstream>
#include "tinyxml2.h"
#include "Map.h"
#include "Stack.h"
#include "Game.h"
#include "GameState.h"
#include "ParamLoader.h"
#include "TextureManager.h"
#include "TileLayer.h"
#include "ObjectLayer.h"

using namespace tinyxml2;

MapParser::MapParser()
{
}

MapParser::~MapParser()
{
}

bool MapParser::parse(std::string filename, std::map<std::string, Environment::Map*>& pMapDict, DataStructure::Stack<Environment::Map>& pMapStack, std::vector<GameObject*>* pObjects, FiniteStateMachine::GameStateID stateID)
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
	XMLElement* pStateNode = pStateRoot->FirstChildElement(FiniteStateMachine::s_stateNames[stateID]);

	//	Checken, ob 'pStateNode' bef�llt wurde
	if (!pStateNode)
	{
		/*	Der Spielzustand besitzt kein <states>-Element und ist demnach leer.
		*	Diesen Fehler loggen wir.
		*/
		TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << " hat keinen \"" << FiniteStateMachine::s_stateNames[stateID] << "\" Zustand." << std::endl << std::endl;

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
		TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << " hat kein <maps>-Element im Zustand \"" << FiniteStateMachine::s_stateNames[stateID] << "\"." << std::endl << std::endl;
	
		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
		return false;
	}

	//	Checken, ob <maps> bef�llt ist
	if (pMapNode->NoChildren())
	{
		//	Lies die Log-Message du Vogel!
		TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << " hat keine Maps im Zustand \"" << FiniteStateMachine::s_stateNames[stateID] << "\"." << std::endl << std::endl;
		
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
			TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << ": Die " << counter << ". Map im Zustand \"" << FiniteStateMachine::s_stateNames[stateID] << "\" hat keine id." << std::endl << std::endl;

			//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
			return false;
		}
		if (mapPath.empty())
		{
			TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << ": Die " << counter << ". Map im Zustand \"" << FiniteStateMachine::s_stateNames[stateID] << "\" hat keinen path." << std::endl << std::endl;

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
			TheGame::Instance()->logError() << "MapParser::parse(): \n\t" << filename << ": Die " << counter << ". Map im Zustand \"" << FiniteStateMachine::s_stateNames[stateID] << "\" konnte nicht geparst werden." << std::endl << std::endl;

			//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
			return false;
		}

		//	ObjectLayer wird erstellt und bef�llt
		Environment::ObjectLayer* pObjectLayer = new Environment::ObjectLayer();


		std::vector<GameObject*>* pCurrentMapObjects = new std::vector<GameObject*>();

		for (auto g : *pObjects)
		{
			if (SDL_GameObject* sdlG = dynamic_cast<SDL_GameObject*>(g))
			{
				if (sdlG->getMapId() == mapId)
				{
					pCurrentMapObjects->push_back(sdlG);
				}
			}
		}

		pObjectLayer->init(pCurrentMapObjects);

		//	Spielobjekte zum 'ObjectLayer' von 'pCurrentMap' hinzuf�gen
		pCurrentMap->addLayer("ObjectLayer", pObjectLayer);

		//	geparste Map dem Dictionary aus Maps hinzuf�gen
		pMapDict.insert(std::pair<std::string, Environment::Map*>(mapId, pCurrentMap));
	}


	//	Pointer l�schen, um einem Speicherleck vorzubeugen.
	delete pDocument;

	//	Das Parsen ist reibungsfrei abgelaufen. Wir geben "true" zur�ck.
	return true;
}

bool MapParser::parseMap(std::string path, Environment::Map* pMap)
{
	//	Variablen zur Speicherung von Mapdaten
	int mapWidth, mapHeight;
	int tilewidth, tileheight;
	ParamLoader params;

	//	Vector zur tempor�ren Speicherung der 'Tileset'-Objekte
	std::vector<Environment::Tileset> tempTilesets;

	//	Mehrdimensionaler Vector aus zu einem Layer geh�rigen Tiles
	std::vector<std::vector<Environment::Tile*>> tempTiles;

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

#pragma region MapData

	//	width
	if (pMapRoot->QueryAttribute("width", &mapWidth))
	{
		TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path <<": Das <map>-Element besitzt kein width-Attribut" << std::endl << std::endl;
		return false;
	}
	//	height
	if (pMapRoot->QueryAttribute("height", &mapHeight))
	{
		TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <map>-Element besitzt kein height-Attribut" << std::endl << std::endl;
		return false;
	}
	//	tilewidth
	if (pMapRoot->QueryAttribute("tilewidth", &tilewidth))
	{
		TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <map>-Element besitzt kein tilewidth-Attribut" << std::endl << std::endl;
		return false;
	}
	//	tileheight
	if (pMapRoot->QueryAttribute("tileheight", &tileheight))
	{
		TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <map>-Element besitzt kein tileheight-Attribut" << std::endl << std::endl;
		return false;
	}

	params.setWidth(mapWidth);
	params.setHeight(mapHeight);

	//	Map mit vorher geparsten Daten initialisieren
	pMap->init(params, tilewidth, tileheight);
#pragma endregion

#pragma region TilesetAcquisition

	//	Hier wird �ber die <tileset>-Elemente iteriert, deren Daten gespeichert werden
	for (XMLElement* e = pMapRoot->FirstChildElement("tileset"); e != nullptr; e = e->NextSiblingElement("tileset"))
	{
		//	Tempor�res Tilesetobjekt
		Environment::Tileset tempTileset;

		/*	Ab hier werden alle Attribute des Tilesetelements 
		 *	aus der XML-Datei geparst.
		 */

		//	firstgid
		if (e->QueryAttribute("firstgid", &tempTileset.firstgid))
		{
			TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <tileset>-Element besitzt kein firstgid-Attribut." << std::endl << std::endl;
			return false;
		}
		//	tilewidth
		if (e->QueryAttribute("tilewidth", &tempTileset.tilewidth))
		{
			TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <tileset>-Element besitzt kein tilewidth-Attribut." << std::endl << std::endl;
			return false;
		}
		//	tileheight
		if (e->QueryAttribute("tileheight", &tempTileset.tileheight))
		{
			TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <tileset>-Element besitzt kein tileheight-Attribut." << std::endl << std::endl;
			return false;
		}
		//	tilecount
		if (e->QueryAttribute("tilecount", &tempTileset.tilecount))
		{
			TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <tileset>-Element besitzt kein tilecount-Attribut." << std::endl << std::endl;
			return false;
		}
		//	numCols
		if (e->QueryAttribute("columns", &tempTileset.numCols))
		{
			TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <tileset>-Element besitzt kein columns-Attribut." << std::endl << std::endl;
			return false;
		}
		//	numRows
		tempTileset.numRows = tempTileset.tilecount / tempTileset.numCols;
		//	name
		tempTileset.name = e->Attribute("name");
		if (tempTileset.name.empty())
		{
			TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <tileset>-Element besitzt kein name-Attribut." << std::endl << std::endl;
			return false;
		}

		//	Die PNG-Datei des Tilesets wird in den 'TextureManager' geladen
		XMLElement* pImageNode = e->FirstChildElement("image");
		if (!pImageNode)
		{
			TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t" << path << ": "<< tempTileset.name << " hat kein <image>-Element." << std::endl << std::endl;
			return false;
		}
		//	source
		std::string tilesetSource = pImageNode->Attribute("source");
		if (tilesetSource.empty())
		{
			TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <image>-Element in " << tempTileset.name << " besitzt kein source-Attribut." << std::endl << std::endl;
			return false;
		}

		/*	Der "TextureManager" l�dt die Textur des Tilesets
		 *	basierend auf dem Namen "name" und der Quelle "tilesetSource".
		 */
		TheTextureManager::Instance()->load((tempTileset.name), "../assets/" + tilesetSource, TheGame::Instance()->getRenderer());

		//	tempor�res Tileset dem Vector aus Tilesets hinzuf�gen
		tempTilesets.push_back(tempTileset);
	}
#pragma endregion

#pragma region TileLayer
	for (XMLElement* e = pMapRoot->FirstChildElement("layer"); e != nullptr; e = e->NextSiblingElement("layer"))
	{
		Environment::TileLayer* pCurrentLayer = new Environment::TileLayer();
		
		//	Daten des Layers parsen
		//	name
		std::string layerName = e->Attribute("name");
		if(layerName.empty())
		{
			TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <layer>-Element besitzt kein name-Attribut." << std::endl << std::endl;
			return false;
		}

		//	dataNode
		XMLElement* dataNode = e->FirstChildElement("data");
		if(!dataNode)
		{
			TheGame::Instance()->logError() << "MapParser::parseMap(): \n\t " << path << ": Das <layer>-Element " << layerName << " besitzt kein <data>-Element." << std::endl << std::endl;
			return false;
		}

		//	Der CSV-Text aus 'dataNode' wird gespeichert ('stringstream' f�r 'std::getline()' - siehe unten)
		std::stringstream dataStream (dataNode->GetText());

		//	Den zweidimensionalen Vector auf gew�nschte Breite & H�he einstellen
		tempTiles.resize(mapHeight);
		for (std::vector<std::vector<Environment::Tile*>>::iterator it = tempTiles.begin(); it != tempTiles.end(); ++it)
			(*it).resize(mapWidth);
		
		/*	Zun�chst parsen wir die 'Id's der Tiles aus dem CSV-Text der 'dataNode'.
		 *	Den zweidimensionalen Vector aus Tiles 'tempTiles' bef�llen wir mit Hilfe dieser 'Id's.
		 *	Wann wir die n�chste Zeile oder Spalte bef�llen m�ssen, erkennen wir anhand der Breite der Map ('mapWidth')
		 *	und eines Z�hlers ('counter'). 
		 */
		int counter = 0;
		for (std::string tempData; std::getline(dataStream, tempData, ','); counter++)
		{
			//	Wenn wir am Ende der Zeile angelangt sind, entfernen wir den Zeilenumbruch ('\n2' --> '2')
			if (counter % mapWidth == 0)
			{
				tempData.erase(std::find(tempData.begin(), tempData.end(), '\n'));
			}

			//	Objekt zur Speicherung der Initialisierungsdaten des aktuellen Tiles
			ParamLoader tileParams;

			//	Initialisierungsdaten des aktuellen Tiles speichern/ausrechnen
			tileParams.setTileId(std::stoi(tempData));				//	'stoi()' steht f�r "string to int"
			tileParams.setWidth(tilewidth);
			tileParams.setHeight(tileheight);
			//	X- und Y-Position werden in der update()-Funktion von 'TileLayer' gesetzt


			//	Objekt zum Tile an der Stelle x = [counter % mapWidth] y = [counter / mapWidth]
			Environment::Tile* pTempTile = new Environment::Tile();
			pTempTile->load(tileParams);

			/*	Beispiele (mapWidth = 20):
			 *		Bei counter = 0:
			 *			tempTiles[0 / 20][0 % 20] -> tempTiles[0][0]
			 *		Bei counter = 20:
			 *			tempTiles[20 / 20][20 % 20] -> tempTiles[1][0]
			 *		Bei counter = 25:
			 *			tempTiles[25 / 20][25 % 20] -> tempTiles[1][5]
			 */
			tempTiles[counter / mapWidth][counter % mapWidth] = pTempTile;
		}
		/*	'currentLayer' wird bei jeder Iteration mit einem neuen dynamischen Objekt bef�llt.
		 *	Es darf nicht gel�scht werden, da es in der Map noch weiterleben muss.
		 *	'currentLayer::init()' bef�llt das aktuelle 'TileLayer'-Objekt mit den Tilesets der Map 
		 *	und einer 2D-Matrix aus Tiles.
		 *	
		 *	Daraufhin wird das nun bef�llte 'currentLayer' der Map mit Hilfe der Methode 'Map::addLayer' 
		 *	in Zusammenhang mit dem Namen des Layers hinzugef�gt.
		 */
		pCurrentLayer->init(tempTilesets, tempTiles);
		pMap->addLayer(layerName, pCurrentLayer);
	}

	
#pragma endregion 

	//	Pointer l�schen, um einem Speicherleck vorzubeugen.
	delete pMapDocument;

	//	Das Parsen ist reibungsfrei abgelaufen. Wir geben "true" zur�ck.
	return true;
}


