#include "ScriptLoader.h"
#include "Game.h"
#include "tinyxml2.h"

ScriptLoader::ScriptLoader()
{}

ScriptLoader::~ScriptLoader()
{}

using namespace tinyxml2;
#define FILENAME "xmlFiles/scripts1.xml"
#define ERR_MESSAGE "Fehler beim Laden der Scripts!"

std::map<std::string, std::string>* ScriptLoader::loadScripts(const char* filename)
{
	//	Das zu befüllende Dictionary (Darf nicht gelöscht werden, da returnt wird)
	std::map<std::string, std::string>* pScriptMap = new std::map<std::string, std::string>();

	//	Die xml Datei, in der die scripts mit ihren IDs gespeichert sind
	XMLDocument* pDocument = new XMLDocument();

	//	Laden des Dokuments (der Dateiname wird hardgecodet sein)
	if (pDocument->LoadFile(filename))
	{
		TheGame::Instance()->logError() << "ScriptLoader::loadScripts(): \n\t" << filename << " konnte nicht geladen werden. " << pDocument->ErrorName() << std::endl << std::endl;
		TheGame::Instance()->emergencyExit(ERR_MESSAGE);
	}

	//	Ermitteln des Wurzelelementes
	XMLElement* pStateRoot = pDocument->RootElement();
	if (!pStateRoot)
	{

		TheGame::Instance()->logError() << "ScriptLoader::loadScripts(): \n\t" << filename << " hat kein <scripts>-Element." << std::endl << std::endl;
		TheGame::Instance()->emergencyExit(ERR_MESSAGE);
	}

	//	Über jedes <script> Element iterieren 
	int counter = 0;
	for (XMLElement* e = pStateRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement(), counter++)
	{
		//	ID und Path aus der XML Datei extrahieren
		const char* id = e->Attribute("id");
		const char* path = e->Attribute("path");

		//	Checken, ob beide Attribute vorhanden waren
		if (!id)
		{
			TheGame::Instance()->logError() << "ScriptLoader::loadScripts(): \n\t" << filename << ": Das " << counter << ". Script hat keine id." << std::endl << std::endl;
			TheGame::Instance()->emergencyExit(ERR_MESSAGE);
		}
		if (!path)
		{
			TheGame::Instance()->logError() << "ScriptLoader::loadScripts(): \n\t" << filename << ": Das " << counter << ". Script hat keinen path." << std::endl << std::endl;
			TheGame::Instance()->emergencyExit(ERR_MESSAGE);
		}

		//	Nun fügen wir id (key) und path (value) dem Dictionary hinzu
		pScriptMap->insert(std::pair<std::string, std::string>(id, path));
	}

	//	Pointer löschen, um einem Speicherleck vorzubeugen.
	delete pDocument;

	//	Falls das Dictionary nach dem befüllen trotzdem leer ist, soll das festgehalten werden
	if (pScriptMap->empty())
	{
		TheGame::Instance()->logError() << "ScriptLoader::loadScripts(): \n\t" << filename << " hat kein <script>-Element." << std::endl << std::endl;
	}

	//	Einen Pointer auf das befüllte Dictionary zurückgeben.
	return pScriptMap;
}


