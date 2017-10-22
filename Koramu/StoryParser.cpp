#include "StoryParser.h"
#include "tinyxml2.h"
#include "Game.h"

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

	//	Pointer löschen, um einem Speicherleck vorzubeugen.
	delete pDocument;
}
