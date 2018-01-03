#include "StoryParser.h"
#include "tinyxml2.h"
#include "Game.h"
#include "Story.h"
#include "Camera.h"
#include "Notebook.h"
#include "Page.h"
#include "ItemList.h"

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
			TheGame::Instance()->logError() << "StoryParser::parseStory(): \n\t" << filename << ": Die " << counterMain << ". main quest hat keine id." << std::endl << std::endl;
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
				TheGame::Instance()->logError() << "StoryParser::parseStory(): \n\t" << filename 
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

	//	Element für die aktuellen Maps (diejenigen auf dem Stapel) erstellen
	XMLElement* pMapStack = pDocument->NewElement("mapStack");
	pRoot->InsertEndChild(pMapStack);

	//	Über alle Maps auf dem Stack iterieren
	for(int i = 0; i < pPlayState->getMapStack()->size(); i++)
	{
		//	Über das "mapDict" des "PlayState"s iterieren, um an die Ids der Maps auf dem Stapel zu kommen
		for(auto const &entry : pPlayState->getMapDict())
		{
			//	Überprüfen, ob die aktuell betrachtete Map des "mapDict"s mit der aktuell betrachteten des Stapel übereinstimmt
			if (entry.second == pPlayState->getMapStack()->at(i))
			{
				//	Element für die aktuell betrachtete "aktive" Map (sie befindet sich auf dem Stapel) erstellen
				XMLElement* pMap = pDocument->NewElement("activeMap");
				//	Id der aktiven Map speichern 
				pMap->SetAttribute("mapId", entry.first.c_str());
				/*	Das "activeMap"-Element als erstes "Kind" des "mapStack"-Elements einfügen. So sind die Maps am Ende in umgekehrter Reihenfolge,
				 *	in Relation zum Stapel, gelistet.
				 */	
				pMapStack->InsertFirstChild(pMap);
			}
		}
		
	}

	//	Element, als dessen "Kinder", die Objekte (ihrer jeweiligen Map zugeordnet) mit Id und Position, festgehalten werden
	XMLElement* pObjectState = pDocument->NewElement("objectState");
	pRoot->InsertEndChild(pObjectState);

	//	Über die Einträge im "mapDict" des "PlayState"s iterieren
	for(auto const &entry : pPlayState->getMapDict())
	{
		//	Element für die aktuell betrachtete Map. Als Kinder werden die Objekte dieser Map mit ihren Positionen festgehalten
		XMLElement* pMap = pDocument->NewElement(entry.first.c_str());
		pObjectState->InsertEndChild(pMap);

		//	Element für das zentrale Objekt der aktuell betrachteten Map erstellen
		XMLElement* pMapCenterObject = pDocument->NewElement("centerObject");
		//	Id dieses aktuell zentralen Objektes festhalten. Wird auf kein Objekt (nullptr) zentriert, so soll "nullptr" gespeichert werden
		if(entry.second->getCenterObject())
			pMapCenterObject->SetAttribute("id", entry.second->getCenterObject()->getUniqueId().c_str());
		else
			pMapCenterObject->SetAttribute("id", "nullptr");
		pMap->InsertEndChild(pMapCenterObject);

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

	//	Notizen des Notizbuchs speichern
	//	Element für den aktuellen Zustand des Notizbuches
	XMLElement* pNotebookState = pDocument->NewElement("notebookState");
	pRoot->InsertEndChild(pNotebookState);

	//	Über die Seiten des Notizbuches iterieren
	for(auto const &page : TheGame::Instance()->getNotebook()->getPages())
	{
		//	Element für die aktuell betrachtete Seite
		XMLElement* pPage = pDocument->NewElement("page");
		pNotebookState->InsertEndChild(pPage);
		//	Über die Zeilen der Seite iterieren
		for (auto const &line : page.getLines())
		{
			//	Element für die aktuell betrachtete Zeile
			XMLElement* pLine = pDocument->NewElement("line");
			//	Text der Zeile des Notizbuches is das "line"-Elment einbetten
			pLine->SetText(line.c_str());
			pPage->InsertEndChild(pLine);
		}
	}

	//	Die Items im Inventar speichern
	XMLElement* pInventoryState = pDocument->NewElement("inventoryState");
	pRoot->InsertEndChild(pInventoryState);

	//	Es wird über jedes Item iteriert
	for (auto& itemPair : TheGame::Instance()->getItemList()->getItems())
	{
		//	Das Tag für das aktuelle Item
		XMLElement* pItem = pDocument->NewElement("item");
		pInventoryState->InsertEndChild(pItem);

		//	Tag für den Namen des aktuellen Items
		XMLElement* pId = pDocument->NewElement("id");
		pId->SetText(itemPair.first->getId().c_str());
		pItem->InsertEndChild(pId);

		//	Tag für die Anzahl des Items
		XMLElement* pCount = pDocument->NewElement("count");
		pCount->SetText(itemPair.second);
		pItem->InsertEndChild(pCount);
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
# pragma endregion 

	//	Setzen der geparsten Werte im "Story"-Objekt von "Game"
	TheGame::Instance()->getStory()->setQuest(mainQuest, partQuest);

	//	Extrahieren der Ids der Maps, die aufgestapelt werden sollen
	std::string mapId;
	XMLElement* pMapStack = pRoot->FirstChildElement("mapStack");
	for(XMLElement* pMap = pMapStack->FirstChildElement("activeMap"); pMap != nullptr; pMap = pMap->NextSiblingElement("activeMap"))
	{
		//	Extrahieren der "mapId" des aktuell betrachteten "activeMap"-Elements
		mapId = pMap->Attribute("mapId");
		if (mapId.empty())
		{
			TheGame::Instance()->logError() << "StoryParser::loadGame(): \n\t " << filename << ": Ein <activeMap>-Element besitzt kein mapId-Attribut." << std::endl << std::endl;
			return false;
		}
		//	Aufstapeln der gewünschten Map, anhand der Id
		pPlayState->pushMap(mapId);
	}

	//	Variablen, in denen im Folgenden die relevanten Daten eines Spielobjekts festgehalten werden
	std::string id;
	float xPos;
	float yPos;

	//	Variable zur Speicherung der Id des Spielobjekts, auf welches die Kamera, auf der entsprechenden Map, zentriert werden soll
	std::string centerObjectId;

	//	Über die "Kinder" von "objectSate" iterieren. Es handelt sich dabei um "Maps", die wiederum die zugehörigen Objekte enthalten
	for(XMLElement* pMap = pRoot->FirstChildElement("objectState")->FirstChildElement(); pMap != nullptr; pMap = pMap->NextSiblingElement())
	{
		//	Extrahieren der Id des zentralen Objekts auf der aktuell betrachteten Map
		XMLElement* pMapCenterObject = pMap->FirstChildElement("centerObject");
		centerObjectId = pMapCenterObject->Attribute("id");
		if (centerObjectId.empty())
		{
			TheGame::Instance()->logError() << "StoryParser::loadGame(): \n\t " << filename << ": Das aktuelle <object>-Element besitzt kein id-Attribut." << std::endl << std::endl;
			return false;
		}
		
		//	Über die "object"-Elemente des aktuellen Map-Elements iterieren
		for(XMLElement* pObjectData = pMap->FirstChildElement("object"); pObjectData != nullptr; pObjectData = pObjectData->NextSiblingElement("object"))
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
				//	Die Id des aktuellen Spielbjekts wird mit der geparsten des "centerObject"s verglichen und es wird geschaut, ob das selbe Object schon als zentrales Objekt gesetz wurde
				if (!pObject->getUniqueId().compare(centerObjectId) && pPlayState->getMapDict()[pMap->Name()]->getCenterObject()->getUniqueId().compare(centerObjectId))
				{
					//	Die Ids sind identisch. Das aktuelle Objekt wird als zentrales Objekt der aktuell betrachteten Map gesetzt
					pPlayState->getMapDict()[pMap->Name()]->setCenterObject(dynamic_cast<SDL_GameObject*>(pObject));
				}
				//	Ist die "centerObjectId" "nullptr", so soll auf kein Objekt (nullptr) zentriert werden
				else if (centerObjectId == "nullptr")
					pPlayState->getMapDict()[pMap->Name()]->setCenterObject(nullptr);

				//	Die Id des aktuellen Spielbjekts wird mit der id des aktuellen "object"-Elements der xml-Datei verglichen
				if(!pObject->getUniqueId().compare(id))
				{
					//	Die Ids sind identisch. Wir setzen die Position des Spielobjekts anhand der geparsten Daten
					SDL_GameObject* pSDLGameObject = dynamic_cast<SDL_GameObject*>(pObject);
					pSDLGameObject->setPosition(xPos, yPos);
				}
			}
		}
	}

	//	Alte Notizen entfernen, da nur die gespeicherten Notizen erscheinen sollen
	TheGame::Instance()->getNotebook()->clear();
	//	Notizen laden
	//	Über alle "page"-Elemente (Kinder des "notebookState"-Elements) iterieren
	for (XMLElement* pPage = pRoot->FirstChildElement("notebookState")->FirstChildElement("page"); pPage != nullptr; pPage = pPage->NextSiblingElement("page"))
	{
		//	Über alle "line"-Elemente der aktuelle betrachteten "Seite" iterieren
		for (XMLElement* pLine = pPage->FirstChildElement("line"); pLine != nullptr; pLine = pLine->NextSiblingElement("line"))
		{
			//	Text aus dem "line"-Element auslesen und zum Notizbuch hinzufügen
			TheGame::Instance()->getNotebook()->addNote(pLine->GetText());
		}
	}

	/*	Das Inventar wird geleert, da der Inhalt nicht abhängig vom GameState ist
	 *	Außerdem wird die Map aus Items (mögliches Items) aktualisiert.
	 */
	TheGame::Instance()->getItemList()->clear();
	TheGame::Instance()->getItemList()->loadItems();

	//	Über alle "item" tags in "inventoryState" iterierens
	for (XMLElement* e = pRoot->FirstChildElement("inventoryState")->FirstChildElement("item"); e != nullptr; e = e->NextSiblingElement("item"))
	{
		//	Das Item wird der Liste hinzugefügt
 		TheGame::Instance()->getItemList()->addItem(e->FirstChildElement("id")->GetText(), e->FirstChildElement("count")->IntText());
	}

	//	Der Spielstand wurde erfolgreich geladen
	return true;
}
