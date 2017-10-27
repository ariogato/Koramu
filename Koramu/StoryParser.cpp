#include "StoryParser.h"
#include "tinyxml2.h"
#include "Game.h"
#include "Story.h"
#include "Camera.h"

StoryParser::StoryParser()
{
}

StoryParser::~StoryParser()
{
}

using namespace tinyxml2;

#define ERR_MESSAGE "Fehler beim Laden der Story!"

void StoryParser::parseStory(std::vector<std::pair<std::string, std::vector<std::string>>>* pQuestList, const char* filename)
{
	//	Die xml Datei, in der die scripts mit ihren IDs gespeichert sind
	XMLDocument* pDocument = new XMLDocument();

	//	Laden des Dokuments (der Dateiname wird hardgecodet sein)
	if (pDocument->LoadFile(filename))
	{
		TheGame::Instance()->logError() << "StoryParser::parseStory(): \n\t" << filename << " konnte nicht geladen werden. " << pDocument->ErrorName() << std::endl << std::endl;
		TheGame::Instance()->emergencyExit(ERR_MESSAGE);
	}

	//	Ermitteln des Wurzelelementes
	XMLElement* pQuestRoot = pDocument->RootElement();
	if (!pQuestRoot)
	{
		TheGame::Instance()->logError() << "StoryParser::parseStory(): \n\t" << filename << " hat kein <quests>-Element." << std::endl << std::endl;
		TheGame::Instance()->emergencyExit(ERR_MESSAGE);
	}

	//	Über jede main quest und die dazugehörigen part quests iterieren
	int counterMain = 0;
	for (XMLElement* e = pQuestRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement(), counterMain++)
	{
		//	Das id Attribut extrahieren
		const char* mainQuestId = e->Attribute("id");

		//	Checken, ob das Attribut vorhanden war
		if (!mainQuestId)
		{
			TheGame::Instance()->logError() << "StoryParser::lparseStoryoadScripts(): \n\t" << filename << ": Die " << counterMain << ". main quest hat keine id." << std::endl << std::endl;
			TheGame::Instance()->emergencyExit(ERR_MESSAGE);
		}

		//	Die Quest Variable deklarieren
		std::pair<std::string, std::vector<std::string>> tempQuests;

		//	Die main quest setzen
		tempQuests.first = mainQuestId;

		int counterPart = 0;
		for (XMLElement* g = e->FirstChildElement(); g != nullptr; g = g->NextSiblingElement(), counterPart++)
		{
			//	Das id Attribut extrahieren
			const char* partQuestId = g->Attribute("id");

			//	Checken, ob das Attribut vorhanden war
			if (!partQuestId)
			{
				TheGame::Instance()->logError() << "StoryParser::lparseStoryoadScripts(): \n\t" << filename 
					<< ": Die " << counterMain << "." << counterPart <<" part quest hat keine id." << std::endl << std::endl;
				TheGame::Instance()->emergencyExit(ERR_MESSAGE);
			}

			//	Die part quest setzen
			tempQuests.second.push_back(partQuestId);
		}

		//	Die Quests in die Liste einfügen
		pQuestList->push_back(tempQuests);
	}

	//	Pointer löschen, um einem Speicherleck vorzubeugen
	delete pDocument;
}

bool StoryParser::saveGame(const char* filename, FiniteStateMachine::GameState* pPlayState)
{
	//	Ein neues Dokument wird erstellt und geöffnet
	XMLDocument* pDocument = new XMLDocument();

	
	//	Laden des Dokuments (der Dateiname wird hardgecodet sein)
	if (pDocument->LoadFile(filename))
	{
		TheGame::Instance()->logError() << "StoryParser::saveGame(): \n\t" << filename << " konnte nicht geladen werden. " << pDocument->ErrorName() << std::endl << std::endl;
		return false;
	}

	//	Ermitteln des Wurzelelementes
	XMLElement* pRoot = pDocument->RootElement();
	if (!pRoot)
	{
		TheGame::Instance()->logError() << "StoryParser::saveGame(): \n\t" << filename << " hat kein <saveState>-Element." << std::endl << std::endl;
		return false;
	}

	/*	Den alten, in der xml-Datei festgehaltenen, Spielstand löschen.
	 *	Das Dokument wird, bis auf das Wurzelelement, komplett neu befüllt. 
	 */
	pRoot->DeleteChildren();

	/*	Die aktuelle main quest und die akutuelle part quests werden als Attribute des Elements "questState" festgehalten.
	 *	Wie man das mithilfe von tinyxml2 in einer xml-Datei erreicht, wird hier einmal exemplarisch vorgestellt. 
	 *	Die Vorgehensweise ist immer die selbe.	
	 */
	//	Erstellen des xml-Elements mit entsprechender Bezeichnung
	XMLElement* pQuestState = pDocument->NewElement("questState");
	//	Setzen der Attribute des Elements mit entspreichenden Attributnamen (Strings müssen zu C-Strings umgewandelt werden!)
	pQuestState->SetAttribute("mainQuest", TheGame::Instance()->getStory()->getMainQuest().c_str());
	pQuestState->SetAttribute("partQuest", TheGame::Instance()->getStory()->getPartQuest().c_str());
	//	Das Element muss abschließend an der gewünschten Stelle ins Dukument eingefügt werden
	//	Hier wird "questState" als "child" des Wurzelelements eingefügt
	pRoot->InsertEndChild(pQuestState);

	//	Element für die aktuelle Map (oberste auf dem Stapel) erstellen. Attribute werden später gesetzt, wenn sie leichter zu extrahieren sind
	XMLElement* pMap = pDocument->NewElement("currentMap");
	pRoot->InsertEndChild(pMap);

	//	Element, in dem die Id des Objekts, auf das die Kamera zentriert wird, fesgehalten wird.
	XMLElement* pCenterObject = pDocument->NewElement("centerObject");
	pCenterObject->SetAttribute("objectId", pPlayState->getCenterObject()->getUniqueId().c_str());
	pRoot->InsertEndChild(pCenterObject);

	//	Element, als dessen "Kinder", die Objekte (ihrer jeweiligen Map zugeordnet) mit Id und Position, festgehalten werden
	XMLElement* pObjectState = pDocument->NewElement("objectState");
	pRoot->InsertEndChild(pObjectState);

	//	Über die Einträge im "mapDict" des "PlayState"s iterieren
	for(auto const &entry : pPlayState->getMapDict())
	{
		//	Die aktuell betrachtete Map wird mit der aktuellen/obersten Map des PlaySates verglichen
		if(entry.second == pPlayState->getCurrentMap())
		{
			//	Die Id der aktuellen Map wird gespeichert
			pMap->SetAttribute("mapId", entry.first.c_str());
		}

		//	Element für die aktuell betrachtete Map. Als Kinder werden die Objekte dieser Map mit ihren Positionen festgehalten
		XMLElement* pMap = pDocument->NewElement(entry.first.c_str());
		pObjectState->InsertEndChild(pMap);

		//	Über die Objekte der Map iterieren
		for(auto const &o : *entry.second->getObjectLayer()->getGameObjects())
		{
			//	Element für das aktuelle Objekt. Es werden die Id, die x-Position und die y-Position festgehalten (evtl. irgendwann noch "currentCol")
			XMLElement* pGameObject = pDocument->NewElement("object");
			pGameObject->SetAttribute("id", o->getUniqueId().c_str());
			pGameObject->SetAttribute("xPos", o->getPosition().getX());
			pGameObject->SetAttribute("yPos", o->getPosition().getY());
			pMap->InsertEndChild(pGameObject);
		}
	}

	//	Zu guter Letzt muss die Datei gespeichert werden, damit die Neuerungen erhalten bleiben
	pDocument->SaveFile(filename);

	//	Der Spielstand wurde erfolgreich gespeichert
	return true;
}

bool StoryParser::loadGame(const char* filename, FiniteStateMachine::GameState* pPlayState)
{
	//	Ein neues Dokument wird erstellt und geöffnet
	XMLDocument* pDocument = new XMLDocument();

	//	Laden des Dokuments (der Dateiname wird hardgecodet sein)
	if (pDocument->LoadFile(filename))
	{
		TheGame::Instance()->logError() << "StoryParser::loadGame(): \n\t" << filename << " konnte nicht geladen werden. " << pDocument->ErrorName() << std::endl << std::endl;
		return false;
	}

	//	Ermitteln des Wurzelelementes
	XMLElement* pRoot = pDocument->RootElement();
	if (!pRoot)
	{
		TheGame::Instance()->logError() << "StoryParser::loadGame(): \n\t" << filename << " hat kein <saveState>-Element." << std::endl << std::endl;
		return false;
	}

	//	Ermtteln der main quest und der part quest aus dem entsprechenden xml-Element
	XMLElement* pQuestState = pRoot->FirstChildElement("questState");
	std::string mainQuest = pQuestState->Attribute("mainQuest");
	std::string partQuest = pQuestState->Attribute("partQuest");

	//	Extrahieren der Id der Map, die aufgestapelt werden soll
	XMLElement* pMap = pRoot->FirstChildElement("currentMap");
	std::string mapId = pMap->Attribute("mapId");

	//	Ermitteln der Id des Objekts, auf das die Kamera zentriert werden soll. 
	XMLElement* pCenterObject = pRoot->FirstChildElement("centerObject");
	std::string centerObjectId = pCenterObject->Attribute("objectId");

# pragma region string validation
	//	Hier wird überprüft, ob die Daten extrahiert werden konnten. Wenn nicht, dann macht es keinen Sinn fortzufahren.
	if (mainQuest.empty())
	{
		TheGame::Instance()->logError() << "StoryParser::loadGame(): \n\t " << filename << ": Das <questState>-Element besitzt kein mainQuest-Attribut." << std::endl << std::endl;
		return false;
	}
	if (partQuest.empty())
	{
		TheGame::Instance()->logError() << "StoryParser::loadGame(): \n\t " << filename << ": Das <questState>-Element besitzt kein mainQuest-Attribut." << std::endl << std::endl;
		return false;
	}
	if (mapId.empty())
	{
		TheGame::Instance()->logError() << "StoryParser::loadGame(): \n\t " << filename << ": Das <currentMap>-Element besitzt kein mapId-Attribut." << std::endl << std::endl;
		return false;
	}
	if (centerObjectId.empty())
	{
		TheGame::Instance()->logError() << "StoryParser::loadGame(): \n\t " << filename << ": Das <centerObject>-Element besitzt kein objectId-Attribut." << std::endl << std::endl;
		return false;
	}
# pragma endregion 
	
	//	Setzen der geparsten Werte im "Story"-Objekt von "Game"
	TheGame::Instance()->getStory()->setQuest(mainQuest, partQuest);

	//	Aufstapeln der gewünschten Map, anhand der Id. Diese Map wird gerendert und geupdated
	pPlayState->pushMap(mapId);
	pPlayState->popMap();
	pPlayState->pushMap(mapId);

	//	Varieblen, in denen im Folgenden die relevanten Daten eines Spielobjekts festgehalten werden
	std::string id;
	float xPos;
	float yPos;

	//	Über die "Kinder" von "objectSate" iterieren. Es handelt sich dabei um "Maps", die wiederum die zugehörigen Objekte enthalten
	for(XMLElement* pMap = pRoot->FirstChildElement("objectState")->FirstChildElement(); pMap != nullptr; pMap = pMap->NextSiblingElement())
	{
		//	Über die "object"-Elemente des aktuellen Map-Elements iterieren
		for(XMLElement* pObjectData = pMap->FirstChildElement(); pObjectData != nullptr; pObjectData = pObjectData->NextSiblingElement("object"))
		{
			//	Parsen der Daten des aktuellen "object"s:
			//	id
			id = pObjectData->Attribute("id");
			if(id.empty())
			{
				TheGame::Instance()->logError() << "StoryParser::loadGame(): \n\t " << filename << ": Das aktuelle <object>-Element besitzt kein id-Attribut." << std::endl << std::endl;
				return false;
			}
			//	xPos
			if(pObjectData->QueryAttribute("xPos", &xPos))
			{
				TheGame::Instance()->logError() << "StoryParser::loadGame(): \n\t " << filename << ": Das aktuelle <object>-Element "<< id <<" besitzt kein xPos-Attribut." << std::endl << std::endl;
				return false;
			}
			//yPos
			if (pObjectData->QueryAttribute("yPos", &yPos))
			{
				TheGame::Instance()->logError() << "StoryParser::loadGame(): \n\t " << filename << ": Das aktuelle <object>-Element " << id << " besitzt kein yPos-Attribut." << std::endl << std::endl;
				return false;
			}

			//	Anhand des Namens des aktuell betrachteten Map-Elemtent wird die entsprechende "Map" des "PlayState"s ermittelt.
			//	Es wird über die Objekte im "ObjectLayer" dieser Map iteriert
			for(auto pObject : *pPlayState->getMapDict()[pMap->Name()]->getObjectLayer()->getGameObjects())
			{
				//	Die Id des aktuellen Spielbjekts wird mit der geparsten des "centerObject"s verglichen
				if(!pObject->getUniqueId().compare(centerObjectId) && !pPlayState->getCenterObject())
				{
					//	Die Ids sind identisch. Das aktuelle Objekt wird als zentrales Objekt des "PlayState" gesetzt
					pPlayState->setCenterObject(dynamic_cast<SDL_GameObject*>(pObject));
				}

				//	Die Id des aktuellen Spielbjekts wird mit der id des aktuellen object-Elements der xml-Datei verglichen
				if(!pObject->getUniqueId().compare(id))
				{
					//	Die Ids sind identisch. Wir setzen die Position des Spielobjekts anhand der geparsten Daten
					SDL_GameObject* pSDLGameObject = dynamic_cast<SDL_GameObject*>(pObject);
					pSDLGameObject->setPosition(xPos, yPos);
				}
			}
		}
	}

	//	Der Spielstand wurde erfolgreich geladen
	return true;
}
