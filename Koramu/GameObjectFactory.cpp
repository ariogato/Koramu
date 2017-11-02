#include "GameObjectFactory.h"
#include "Game.h"

//	Wichtig für Singleton-Klasse
GameObjectFactory* GameObjectFactory::s_pInstance = nullptr;


GameObjectFactory::GameObjectFactory()
{
}

GameObjectFactory::~GameObjectFactory()
{
	/*
	*	Sobald man mindestens ein dynamisch alloziiertes Objekt
	*	(Schema: "pointer = new class()") als Member-Variable hat
	*	MUSS man einen Destruktor schreiben,
	*	der diese löscht. Sonst entsteht ein übles Speicherleck!!!
	*/
}

bool GameObjectFactory::registerType(std::string id, BaseCreator* pCreator)
{
	//	Ermitteln ob der Creator bereits existiert
	if (m_creatorMap.count(id))
	{
		//	Der Creator wird ab hier nicht mehr gebraucht
		delete pCreator;

		//	Der Typ existiert bereits -> es muss nichts gemacht werden
		return true;
	}

	//	Die CreatorMap um den neuen Typen ertweitern
	m_creatorMap.insert( std::pair<std::string, BaseCreator*> (id, pCreator) );

	TheGame::Instance()->logStandard() << "GameObjectFactory::registerType(): \n\tNeuer Typ hinzugefuegt: " << id << std::endl << std::endl;

	//	Der neue Typ wurde erfolgreich hinzugefügt
	return true;
}

GameObject* GameObjectFactory::create(std::string id)
{
	//	Ermitteln ob der Creator existiert
	if (!m_creatorMap.count(id))
	{
		TheGame::Instance()->logError() << "GameObjectFactory::create(): \n\tAngeforderten Typ nicht gefunden: " << id << std::endl << std::endl;

		return nullptr;
	}

	/*	Hier wird das angeforderte Objekt über die dazugehörige 
	 *	Creator-Klasse erstellt und dem Parser zurückgegeben.
	 *	Der Parser fügt dieses Objekt dann dem zu parsenden State hinzu.
	 */
	return m_creatorMap[id]->createGameObject();
}

bool GameObjectFactory::typeExists(std::string id)
{
	//	Überprüfen, ob die übergebene "id" (key) in "m_creatorMap" existiert
	if (m_creatorMap.count(id))
	{
		//	Die "id" existiert
		return true;
	}
	//	Die "id" existiert nicht
	return false;
}

void GameObjectFactory::destroy()
{
	//	Den Destruktor aufrufen
	delete s_pInstance;

	//	Nochmal - weil es gute Programmierpraxis ist - die Instanz = 'nullptr' setzen
	s_pInstance = nullptr;
}
